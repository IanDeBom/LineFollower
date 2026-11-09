// Lijnvolger - finaal programma
// hardware: eigen PCB met ATmega32U4 (bord in de Arduino IDE: Arduino Leonardo)
//
// - cyclische uitvoering op vaste tijdstippen (zoals een PLC), cyclustijd instelbaar
// - 8 sensoren, gemeten met IR-leds uit en aan (verschil = ongevoelig voor omgevingslicht)
// - kalibratie van de sensoren, opgeslagen in EEPROM
// - PID-regeling, parameters instelbaar en opgeslagen in EEPROM
// - commando's via USB en via bluetooth (BT05), zonder de regeling te blokkeren
// - start/stop via de drukknop (interrupt) of via een commando
//
// basis: cyclus-programma van de leerkracht (SerialCommand + EEPROMAnything)

#include "SerialCommand.h"
#include "EEPROMAnything.h"
#include "TimeoutStream.h"
#include <avr/wdt.h>

// ---------------------------------------------------------------------------
// pinnen (zie elektrisch schema)
// ---------------------------------------------------------------------------
const uint8_t SENSOR_COUNT = 8;
const uint8_t SENSOR_PINS[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5, A6, A11};  // Sen1..Sen8

// fysieke volgorde van links naar rechts (kijkend in rijrichting), als index in SENSOR_PINS
// controleer met het testprogramma en pas aan indien nodig, bv. {5, 4, 3, 2, 1, 0, 6, 7}
const uint8_t SENSOR_ORDER[SENSOR_COUNT] = {0, 1, 2, 3, 4, 5, 6, 7};

const uint8_t PIN_IR_ON  = 11;  // PB7, hoog = IR-leds aan (Q1)
const uint8_t PIN_AIN1   = 9;   // PB5, Timer1
const uint8_t PIN_AIN2   = 10;  // PB6, Timer1
const uint8_t PIN_BIN1   = 13;  // PC7, Timer4
const uint8_t PIN_BIN2   = 6;   // PD7, Timer4
const uint8_t PIN_NSLEEP = 2;   // PD1, hoog = DRV8833 aan
const uint8_t PIN_NFAULT = 30;  // PD5, laag = fout DRV8833 (10k pull-up)
const uint8_t PIN_START  = 3;   // PD0, INT0, drukknop naar GND (10k pull-up)
const uint8_t PIN_VBAT   = A8;  // PB4, spanningsdeler 100k / 100k

// motoraansluiting: controleer met het testprogramma
const bool MOTOR_A_IS_LEFT = true;   // motor A (AOUT1/2) is de linkermotor
const bool INVERT_LEFT     = false;  // true als de linkermotor achteruit draait bij een positieve snelheid
const bool INVERT_RIGHT    = false;  // idem voor de rechtermotor

// vaste instellingen
const unsigned int  LED_SETTLE_US   = 250;    // wachttijd na IR-leds aan voor de meting
const unsigned long DEBOUNCE_MS     = 250;    // minimale tijd tussen 2 drukken op de startknop
const unsigned long CAL_TIME_MS     = 5000;   // duur van de kalibratie
const int           CAL_SPIN_POWER  = 70;     // draaisnelheid bij "cal draai"
const int           CAL_MIN_RANGE   = 50;     // minimaal verschil wit/zwart voor een geldige kalibratie
const float         VBAT_MIN        = 6.2;    // onder deze spanning stopt de robot (2S Li-Ion)
const float         VBAT_PRESENT    = 3.0;    // daaronder: geen batterij (enkel USB), geen controle

// ---------------------------------------------------------------------------
// parameters in EEPROM
// ---------------------------------------------------------------------------
const uint16_t PARAM_MAGIC = 0x4C46;  // "LF": controle of het EEPROM al geldige parameters bevat

struct param_t
{
  uint16_t magic;
  unsigned long cycleTime;      // cyclustijd in µs
  float kp, ki, kd;             // PID
  int16_t power;                // basissnelheid 0..255
  int16_t threshold;            // genormaliseerde waarde (0..1000) vanaf wanneer een sensor de lijn ziet
  int16_t calMin[SENSOR_COUNT]; // kalibratie: zwart (weinig reflectie)
  int16_t calMax[SENSOR_COUNT]; // kalibratie: wit (veel reflectie)
} params;

void setDefaults(bool keepCalibration)
{
  params.magic = PARAM_MAGIC;
  params.cycleTime = 5000;
  params.kp = 40.0;
  params.ki = 0.0;
  params.kd = 0.5;
  params.power = 100;
  params.threshold = 300;
  if (!keepCalibration)
  {
    for (uint8_t i = 0; i < SENSOR_COUNT; i++)
    {
      params.calMin[i] = 0;
      params.calMax[i] = 0;
    }
  }
}

// ---------------------------------------------------------------------------
// communicatie: USB (Serial) en bluetooth (Serial1, BT05)
// ---------------------------------------------------------------------------
TimeoutStream usbStream(Serial, 50);
TimeoutStream btStream(Serial1, 50);
SerialCommand usbCmd(usbStream);
SerialCommand btCmd(btStream);
Stream* out = &Serial;  // poort waarop het huidige commando binnenkwam: daar gaat het antwoord naartoe

// ---------------------------------------------------------------------------
// toestand
// ---------------------------------------------------------------------------
volatile bool running = false;
volatile unsigned long lastPress = 0;
bool wasRunning = false;

enum Mode { MODE_IDLE, MODE_CALIBRATE } mode = MODE_IDLE;
unsigned long calStart;
bool calSpin;

unsigned long previous, calculationTime;
unsigned long streamInterval = 0, lastStream = 0;

int raw[SENSOR_COUNT];         // reflectie per sensor (fysieke volgorde): hoog = wit
int normalized[SENSOR_COUNT];  // 0 = wit, 1000 = zwart (na kalibratie)
long position = 3500;          // 0 = lijn helemaal links, 7000 = helemaal rechts
bool lineSeen = false;
float error = 0, lastError = 0, integral = 0;
int speedLeft = 0, speedRight = 0;
float vbat = 0;
uint16_t cycleCount = 0;

// ---------------------------------------------------------------------------
// berichten: niet-blokkerend versturen naar USB en bluetooth
// ---------------------------------------------------------------------------
void sendAll(const char* msg)
{
  size_t len = strlen(msg) + 2;
  // Serial.dtr() = terminal open op de pc (niet "if (Serial)": die wacht telkens 10 ms)
  if (Serial.dtr() && Serial.availableForWrite() >= (int)len) Serial.println(msg);
  if (Serial1.availableForWrite() >= (int)len) Serial1.println(msg);
}

// ---------------------------------------------------------------------------
// sensoren
// ---------------------------------------------------------------------------
void readSensors()
{
  int ambient[SENSOR_COUNT];
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) ambient[i] = analogRead(SENSOR_PINS[SENSOR_ORDER[i]]);

  digitalWrite(PIN_IR_ON, HIGH);
  delayMicroseconds(LED_SETTLE_US);
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    int lit = analogRead(SENSOR_PINS[SENSOR_ORDER[i]]);
    raw[i] = max(ambient[i] - lit, 0);  // meer reflectie = lagere spanning met leds aan
  }
  digitalWrite(PIN_IR_ON, LOW);
}

bool isCalibrated()
{
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    if (params.calMax[i] - params.calMin[i] < CAL_MIN_RANGE) return false;
  }
  return true;
}

void normalizeSensors()
{
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    long range = params.calMax[i] - params.calMin[i];
    if (range <= 0) range = 1;
    long value = (long)(params.calMax[i] - raw[i]) * 1000 / range;
    normalized[i] = constrain(value, 0, 1000);
  }
}

// gewogen gemiddelde van de sensoren die de lijn zien
void calculatePosition()
{
  long sum = 0, weighted = 0;
  lineSeen = false;
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    if (normalized[i] >= params.threshold) lineSeen = true;
    if (normalized[i] > 50)  // ruis negeren
    {
      sum += normalized[i];
      weighted += (long)normalized[i] * i * 1000;
    }
  }
  // lijn kwijt (bv. onderbreking of te scherpe bocht): laatste positie behouden
  if (lineSeen && sum > 0) position = weighted / sum;
}

// ---------------------------------------------------------------------------
// motoren (DRV8833, "slow decay": snelheid lineairder en remt actief af)
// ---------------------------------------------------------------------------
void driveChannel(uint8_t in1, uint8_t in2, int speed)
{
  speed = constrain(speed, -255, 255);
  if (speed >= 0)
  {
    digitalWrite(in1, HIGH);
    analogWrite(in2, 255 - speed);
  }
  else
  {
    digitalWrite(in2, HIGH);
    analogWrite(in1, 255 + speed);
  }
}

void setMotors(int left, int right)
{
  speedLeft = constrain(left, -255, 255);
  speedRight = constrain(right, -255, 255);
  int l = INVERT_LEFT ? -speedLeft : speedLeft;
  int r = INVERT_RIGHT ? -speedRight : speedRight;
  if (MOTOR_A_IS_LEFT)
  {
    driveChannel(PIN_AIN1, PIN_AIN2, l);
    driveChannel(PIN_BIN1, PIN_BIN2, r);
  }
  else
  {
    driveChannel(PIN_AIN1, PIN_AIN2, r);
    driveChannel(PIN_BIN1, PIN_BIN2, l);
  }
}

void motorsOff()
{
  digitalWrite(PIN_NSLEEP, LOW);  // driver uit: uitgangen hoogohmig, motoren bollen uit
  setMotors(0, 0);
}

// ---------------------------------------------------------------------------
// start / stop
// ---------------------------------------------------------------------------
void onStartButton()  // interrupt: zo kort mogelijk
{
  unsigned long now = millis();
  if (now - lastPress < DEBOUNCE_MS) return;
  lastPress = now;
  running = !running;
  if (!running) digitalWrite(PIN_NSLEEP, LOW);  // stoppen gebeurt onmiddellijk
}

void stopRobot(const char* reason)
{
  running = false;
  motorsOff();
  sendAll(reason);
}

// ---------------------------------------------------------------------------
// cyclus
// ---------------------------------------------------------------------------
void cycle()
{
  // batterijspanning: elke 50 cycli meten volstaat
  if (cycleCount++ % 50 == 0) vbat = analogRead(PIN_VBAT) * (5.0 / 1023.0) * 2.0;

  readSensors();

  // ---- kalibratie ----
  if (mode == MODE_CALIBRATE)
  {
    for (uint8_t i = 0; i < SENSOR_COUNT; i++)
    {
      if (raw[i] < params.calMin[i]) params.calMin[i] = raw[i];
      if (raw[i] > params.calMax[i]) params.calMax[i] = raw[i];
    }
    unsigned long elapsed = millis() - calStart;
    if (calSpin)  // eerste helft naar links draaien, tweede helft naar rechts
    {
      int s = elapsed < CAL_TIME_MS / 2 ? CAL_SPIN_POWER : -CAL_SPIN_POWER;
      setMotors(-s, s);
    }
    if (elapsed >= CAL_TIME_MS)
    {
      mode = MODE_IDLE;
      motorsOff();
      if (isCalibrated())
      {
        EEPROM_writeAnything(0, params);
        sendAll("kalibratie klaar en opgeslagen");
      }
      else
      {
        sendAll("kalibratie mislukt: niet elke sensor zag wit en zwart");
      }
    }
    return;
  }

  normalizeSensors();
  calculatePosition();

  // ---- start / stop ----
  if (running && !wasRunning)  // net gestart
  {
    if (!isCalibrated())
    {
      stopRobot("niet gestart: eerst kalibreren (cal)");
    }
    else
    {
      integral = 0;
      lastError = 0;
      digitalWrite(PIN_NSLEEP, HIGH);
      sendAll("gestart");
    }
  }
  else if (!running && wasRunning)  // net gestopt (knop of commando)
  {
    motorsOff();
    sendAll("gestopt");
  }

  // ---- veiligheid ----
  if (running && digitalRead(PIN_NFAULT) == LOW) stopRobot("gestopt: fout DRV8833 (overstroom / oververhitting)");
  if (running && vbat > VBAT_PRESENT && vbat < VBAT_MIN) stopRobot("gestopt: batterij leeg");

  wasRunning = running;

  // ---- PID-regeling ----
  if (running)
  {
    float dt = params.cycleTime / 1000000.0;
    error = (position - 3500) / 1000.0;  // in "sensorafstanden": -3,5 .. 3,5, positief = lijn rechts

    integral += error * dt;
    if (params.ki > 0)  // anti-windup: I-bijdrage begrenzen tot de volle motorsnelheid
    {
      float limit = 255.0 / params.ki;
      integral = constrain(integral, -limit, limit);
    }
    float derivative = (error - lastError) / dt;
    lastError = error;

    float output = params.kp * error + params.ki * integral + params.kd * derivative;
    setMotors(params.power + output, params.power - output);
  }

  // ---- status doorsturen ----
  if (streamInterval > 0 && millis() - lastStream >= streamInterval)
  {
    lastStream = millis();
    char msg[64];
    snprintf(msg, sizeof(msg), "pos %ld lijn %d L %d R %d bat %d", position, lineSeen, speedLeft, speedRight, (int)(vbat * 100));
    sendAll(msg);
  }
}

// ---------------------------------------------------------------------------
// setup / loop
// ---------------------------------------------------------------------------
void setup()
{
  // de USB-code van de Leonardo gebruikt PD5 als TX-led (uitgang): terug als ingang voor nFault
  pinMode(PIN_NFAULT, INPUT);

  pinMode(PIN_NSLEEP, OUTPUT);
  digitalWrite(PIN_NSLEEP, LOW);
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);
  pinMode(PIN_IR_ON, OUTPUT);
  digitalWrite(PIN_IR_ON, LOW);
  pinMode(PIN_START, INPUT);  // externe pull-up van 10k

  // PWM van ± 490 Hz naar ± 31 kHz: geen hoorbaar fluiten van de motoren
  TCCR1B = (TCCR1B & 0b11111000) | 0b001;   // Timer1 (D9, D10): prescaler 1
  TCCR4B = (TCCR4B & 0b11110000) | 0b0001;  // Timer4 (D13, D6): prescaler 1
  setMotors(0, 0);

  Serial.begin(115200);  // USB
  Serial1.begin(9600);   // BT05

  EEPROM_readAnything(0, params);
  if (params.magic != PARAM_MAGIC)  // leeg of oud EEPROM: standaardwaarden
  {
    setDefaults(false);
    EEPROM_writeAnything(0, params);
  }

  addCommands(usbCmd);
  addCommands(btCmd);

  attachInterrupt(digitalPinToInterrupt(PIN_START), onStartButton, FALLING);

  sendAll("ready");
}

void loop()
{
  // commando's inlezen: niet-blokkerend, het antwoord gaat naar de poort van het commando
  out = &Serial;
  usbCmd.readSerial();
  out = &Serial1;
  btCmd.readSerial();

  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;
    cycle();
  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

// ---------------------------------------------------------------------------
// commando's
// ---------------------------------------------------------------------------
void addCommands(SerialCommand& cmd)
{
  cmd.addCommand("set", onSet);
  cmd.addCommand("debug", onDebug);
  cmd.addCommand("start", onStart);
  cmd.addCommand("stop", onStop);
  cmd.addCommand("cal", onCalibrate);
  cmd.addCommand("sensors", onSensors);
  cmd.addCommand("stream", onStream);
  cmd.addCommand("default", onDefault);
  cmd.addCommand("reset", onReset);
  cmd.addCommand("help", onHelp);
  cmd.setDefaultHandler(onUnknownCommand);
}

SerialCommand& currentCmd() { return out == &Serial ? usbCmd : btCmd; }

void onUnknownCommand(char* command)
{
  out->print(F("onbekend commando: \""));
  out->print(command);
  out->println(F("\" (typ help)"));
}

void onSet()
{
  char* param = currentCmd().next();
  char* value = currentCmd().next();
  if (param == NULL || value == NULL)
  {
    out->println(F("gebruik: set <cycle|kp|ki|kd|power|threshold> <waarde>"));
    return;
  }

  if      (strcmp(param, "cycle") == 0)     params.cycleTime = constrain(atol(value), 1000, 50000);
  else if (strcmp(param, "kp") == 0)        params.kp = atof(value);
  else if (strcmp(param, "ki") == 0)        params.ki = atof(value);
  else if (strcmp(param, "kd") == 0)        params.kd = atof(value);
  else if (strcmp(param, "power") == 0)     params.power = constrain(atoi(value), 0, 255);
  else if (strcmp(param, "threshold") == 0) params.threshold = constrain(atoi(value), 0, 1000);
  else
  {
    out->print(F("onbekende parameter: "));
    out->println(param);
    return;
  }

  EEPROM_writeAnything(0, params);
  out->print(F("ok "));
  out->println(param);
}

void onDebug()
{
  if (running)  // kort houden tijdens het rijden (bluetooth is traag)
  {
    out->print(F("rijdt, pos "));
    out->println(position);
    return;
  }
  out->print(F("cycle time: "));      out->println(params.cycleTime);
  out->print(F("kp: "));              out->println(params.kp, 3);
  out->print(F("ki: "));              out->println(params.ki, 3);
  out->print(F("kd: "));              out->println(params.kd, 3);
  out->print(F("power: "));           out->println(params.power);
  out->print(F("threshold: "));       out->println(params.threshold);
  out->print(F("gekalibreerd: "));    out->println(isCalibrated() ? F("ja") : F("nee"));
  out->print(F("cal zwart: "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) { out->print(params.calMin[i]); out->print(' '); }
  out->println();
  out->print(F("cal wit: "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) { out->print(params.calMax[i]); out->print(' '); }
  out->println();
  out->print(F("batterij: "));        out->print(vbat, 2); out->println(F(" V"));
  out->print(F("calculation time: ")); out->println(calculationTime);
  calculationTime = 0;
}

void onStart()
{
  if (mode == MODE_CALIBRATE) { out->println(F("eerst kalibratie afwachten")); return; }
  running = true;  // de cyclus controleert de kalibratie en start de motoren
}

void onStop()
{
  running = false;
  digitalWrite(PIN_NSLEEP, LOW);
  if (mode == MODE_CALIBRATE)
  {
    mode = MODE_IDLE;
    motorsOff();
    EEPROM_readAnything(0, params);  // onvolledige kalibratie niet bewaren
    out->println(F("kalibratie afgebroken"));
  }
}

void onCalibrate()
{
  if (running) { out->println(F("eerst stoppen")); return; }
  char* arg = currentCmd().next();
  calSpin = (arg != NULL && strcmp(arg, "draai") == 0);

  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    params.calMin[i] = 1023;
    params.calMax[i] = 0;
  }
  if (calSpin) digitalWrite(PIN_NSLEEP, HIGH);
  calStart = millis();
  mode = MODE_CALIBRATE;
  out->println(calSpin ? F("kalibratie 5 s: robot draait over de lijn")
                       : F("kalibratie 5 s: beweeg de sensoren over wit en zwart"));
}

void onSensors()
{
  out->print(F("norm "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) { out->print(normalized[i]); out->print(' '); }
  out->print(F("| raw "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) { out->print(raw[i]); out->print(' '); }
  out->print(F("| pos "));
  out->println(position);
}

void onStream()
{
  char* arg = currentCmd().next();
  streamInterval = arg == NULL ? 0 : constrain(atol(arg), 0, 10000);
  if (streamInterval > 0 && streamInterval < 100) streamInterval = 100;  // bluetooth niet overladen
  out->print(F("stream "));
  out->println(streamInterval > 0 ? F("aan") : F("uit"));
}

void onDefault()
{
  if (running) { out->println(F("eerst stoppen")); return; }
  setDefaults(true);  // kalibratie blijft behouden
  EEPROM_writeAnything(0, params);
  out->println(F("standaardwaarden hersteld"));
}

void onReset()
{
  out->println(F("herstarten..."));
  out->flush();
  motorsOff();
  wdt_enable(WDTO_15MS);  // watchdog laat de microcontroller herstarten
  while (true) {}
}

void onHelp()
{
  out->println(F("set <cycle|kp|ki|kd|power|threshold> <waarde>"));
  out->println(F("start | stop | cal [draai] | sensors"));
  out->println(F("stream <ms> (0 = uit) | debug | default | reset"));
}

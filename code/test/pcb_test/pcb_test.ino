// Lijnvolger - testprogramma voor de PCB
// test elk onderdeel afzonderlijk: sensoren, IR-leds, motoren, drukknop, batterijmeting,
// foutmelding DRV8833, EEPROM en bluetooth
// hardware: eigen PCB met ATmega32U4 (bord in de Arduino IDE: Arduino Leonardo)
//
// commando's via USB (seriële monitor, 115200) of via bluetooth (BT05): typ "help"

#include "SerialCommand.h"
#include "EEPROMAnything.h"
#include "TimeoutStream.h"

// pinnen (zie elektrisch schema), zelfde als in het finale programma
const uint8_t SENSOR_COUNT = 8;
const uint8_t SENSOR_PINS[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5, A6, A11};  // Sen1..Sen8
const uint8_t PIN_IR_ON  = 11;  // PB7
const uint8_t PIN_AIN1   = 9;   // PB5
const uint8_t PIN_AIN2   = 10;  // PB6
const uint8_t PIN_BIN1   = 13;  // PC7
const uint8_t PIN_BIN2   = 6;   // PD7
const uint8_t PIN_NSLEEP = 2;   // PD1
const uint8_t PIN_NFAULT = 30;  // PD5
const uint8_t PIN_START  = 3;   // PD0, INT0
const uint8_t PIN_VBAT   = A8;  // PB4

const unsigned int  LED_SETTLE_US = 250;
const unsigned long DEBOUNCE_MS   = 250;
const int EEPROM_TEST_ADDRESS     = 1000;  // achteraan het EEPROM, buiten de parameters van het finale programma

TimeoutStream usbStream(Serial, 50);
TimeoutStream btStream(Serial1, 50);
SerialCommand usbCmd(usbStream);
SerialCommand btCmd(btStream);
Stream* out = &Serial;

volatile unsigned int buttonCount = 0;
volatile unsigned long lastPress = 0;
unsigned int lastButtonCount = 0;
bool lastFault = false;

bool sensorStream = false;
Stream* sensorOut = &Serial;
unsigned long lastSensorPrint = 0;

// ---------- hulpfuncties ----------
void driveChannel(uint8_t in1, uint8_t in2, int speed)  // zelfde aansturing als het finale programma
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

void motorsOff()
{
  digitalWrite(PIN_NSLEEP, LOW);
  driveChannel(PIN_AIN1, PIN_AIN2, 0);
  driveChannel(PIN_BIN1, PIN_BIN2, 0);
}

float readBattery()
{
  return analogRead(PIN_VBAT) * (5.0 / 1023.0) * 2.0;
}

void printSensors(Stream* s)
{
  int ambient[SENSOR_COUNT], lit[SENSOR_COUNT];
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) ambient[i] = analogRead(SENSOR_PINS[i]);
  digitalWrite(PIN_IR_ON, HIGH);
  delayMicroseconds(LED_SETTLE_US);
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) lit[i] = analogRead(SENSOR_PINS[i]);
  digitalWrite(PIN_IR_ON, LOW);

  // formaat "s1:123 s2:..." = reflectie (leds uit - leds aan), ook leesbaar in de seriële plotter
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    s->print('s');
    s->print(i + 1);
    s->print(':');
    s->print(max(ambient[i] - lit[i], 0));
    s->print(i < SENSOR_COUNT - 1 ? ' ' : '\n');
  }
}

void onButton()
{
  unsigned long now = millis();
  if (now - lastPress < DEBOUNCE_MS) return;
  lastPress = now;
  buttonCount++;
}

void sendAll(const char* msg)
{
  if (Serial.dtr()) Serial.println(msg);
  Serial1.println(msg);
}

// ---------- setup / loop ----------
void setup()
{
  pinMode(PIN_NFAULT, INPUT);  // PD5 wordt door de USB-code als TX-led gebruikt: terug als ingang
  pinMode(PIN_NSLEEP, OUTPUT);
  digitalWrite(PIN_NSLEEP, LOW);
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);
  pinMode(PIN_IR_ON, OUTPUT);
  digitalWrite(PIN_IR_ON, LOW);
  pinMode(PIN_START, INPUT);

  TCCR1B = (TCCR1B & 0b11111000) | 0b001;   // PWM ± 31 kHz, zoals het finale programma
  TCCR4B = (TCCR4B & 0b11110000) | 0b0001;
  motorsOff();

  Serial.begin(115200);
  Serial1.begin(9600);

  addCommands(usbCmd);
  addCommands(btCmd);

  attachInterrupt(digitalPinToInterrupt(PIN_START), onButton, FALLING);

  sendAll("testprogramma lijnvolger - typ help");
}

void loop()
{
  out = &Serial;
  usbCmd.readSerial();
  out = &Serial1;
  btCmd.readSerial();

  // drukknop: melding bij elke druk
  noInterrupts();
  unsigned int count = buttonCount;
  interrupts();
  if (count != lastButtonCount)
  {
    lastButtonCount = count;
    char msg[32];
    snprintf(msg, sizeof(msg), "knop ingedrukt (%u)", count);
    sendAll(msg);
  }

  // DRV8833 foutmelding
  bool fault = digitalRead(PIN_NFAULT) == LOW;
  if (fault != lastFault)
  {
    lastFault = fault;
    sendAll(fault ? "nFault: FOUT (laag)" : "nFault: ok (hoog)");
  }

  // sensoren doorlopend tonen
  if (sensorStream && millis() - lastSensorPrint >= 200)
  {
    lastSensorPrint = millis();
    printSensors(sensorOut);
  }
}

// ---------- commando's ----------
void addCommands(SerialCommand& cmd)
{
  cmd.addCommand("sensors", onSensors);
  cmd.addCommand("ir", onIr);
  cmd.addCommand("motor", onMotor);
  cmd.addCommand("knop", onKnop);
  cmd.addCommand("batterij", onBatterij);
  cmd.addCommand("fault", onFault);
  cmd.addCommand("eeprom", onEeprom);
  cmd.addCommand("ping", onPing);
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

// sensors        1 meting
// sensors aan    elke 200 ms een meting (ook voor de seriële plotter)
// sensors uit    stoppen
void onSensors()
{
  char* arg = currentCmd().next();
  if (arg != NULL && strcmp(arg, "aan") == 0)
  {
    sensorStream = true;
    sensorOut = out;
  }
  else if (arg != NULL && strcmp(arg, "uit") == 0)
  {
    sensorStream = false;
  }
  else
  {
    printSensors(out);
  }
}

// ir aan / ir uit: IR-leds vast aan of uit (met een gsm-camera zichtbaar als paars licht)
void onIr()
{
  char* arg = currentCmd().next();
  bool on = arg != NULL && strcmp(arg, "aan") == 0;
  digitalWrite(PIN_IR_ON, on);
  out->print(F("IR-leds "));
  out->println(on ? F("aan") : F("uit"));
}

// motor a <-255..255>, motor b <-255..255>, motor stop
void onMotor()
{
  char* which = currentCmd().next();
  char* value = currentCmd().next();
  if (which != NULL && strcmp(which, "stop") == 0)
  {
    motorsOff();
    out->println(F("motoren uit"));
    return;
  }
  if (which == NULL || value == NULL || (strcmp(which, "a") != 0 && strcmp(which, "b") != 0))
  {
    out->println(F("gebruik: motor <a|b> <-255..255> of motor stop"));
    return;
  }
  int speed = constrain(atoi(value), -255, 255);
  digitalWrite(PIN_NSLEEP, HIGH);
  if (strcmp(which, "a") == 0) driveChannel(PIN_AIN1, PIN_AIN2, speed);
  else                         driveChannel(PIN_BIN1, PIN_BIN2, speed);
  out->print(F("motor "));
  out->print(which);
  out->print(' ');
  out->println(speed);
}

void onKnop()
{
  out->print(F("knop: "));
  out->print(digitalRead(PIN_START) == LOW ? F("ingedrukt") : F("los"));
  out->print(F(", aantal keer ingedrukt: "));
  out->println(buttonCount);
}

void onBatterij()
{
  out->print(F("batterij: "));
  out->print(readBattery(), 2);
  out->println(F(" V"));
}

void onFault()
{
  out->print(F("nFault: "));
  out->println(digitalRead(PIN_NFAULT) == LOW ? F("FOUT (laag)") : F("ok (hoog)"));
}

// schrijft een testwaarde, leest ze terug en zet de oude waarde terug
void onEeprom()
{
  unsigned long original, check;
  const unsigned long pattern = 0xA5C3F00Ful;
  EEPROM_readAnything(EEPROM_TEST_ADDRESS, original);
  EEPROM_writeAnything(EEPROM_TEST_ADDRESS, pattern);
  EEPROM_readAnything(EEPROM_TEST_ADDRESS, check);
  EEPROM_writeAnything(EEPROM_TEST_ADDRESS, original);
  out->println(check == pattern ? F("EEPROM ok") : F("EEPROM FOUT"));
}

void onPing()
{
  out->println(F("pong"));
}

void onHelp()
{
  out->println(F("sensors [aan|uit] | ir <aan|uit>"));
  out->println(F("motor <a|b> <-255..255> | motor stop"));
  out->println(F("knop | batterij | fault | eeprom | ping"));
}

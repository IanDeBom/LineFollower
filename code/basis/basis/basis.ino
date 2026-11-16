// Lijnvolger - basisprogramma
// kapstok voor het finale programma, gebaseerd op het cyclus-programma van de leerkracht
// hardware: eigen PCB met ATmega32U4 (bord in de Arduino IDE: Arduino Leonardo)
//
// - commando's inlezen en status terugsturen via USB en bluetooth (BT05), zonder te blokkeren
// - instelbare parameters opgeslagen in EEPROM (overleven een reset en spanningsloos maken)
// - loop op vaste tijdstippen (cyclus, vergelijkbaar met een PLC)
// - start/stop via commando of via de drukknop (externe interrupt)
// - voorbeeld in de cyclus: de IR-leds knipperen zolang de robot "gestart" is
//   (IR-licht is onzichtbaar voor het oog, maar wel zichtbaar met een gsm-camera)

#include "SerialCommand.h"
#include "EEPROMAnything.h"
#include "TimeoutStream.h"

#define Baudrate   115200  // USB
#define BtBaudrate 9600    // BT05

// pinnen (zie elektrisch schema)
const uint8_t PIN_START  = 3;   // PD0, INT0, drukknop naar GND (10k pull-up)
const uint8_t PIN_LED    = 11;  // PB7, IR-leds van de sensoren (Q1)
const uint8_t PIN_NSLEEP = 2;   // PD1, DRV8833: laag houden = motoren uit
const uint8_t PIN_NFAULT = 30;  // PD5, DRV8833 foutmelding

const unsigned long DEBOUNCE_MS = 250;
const uint16_t PARAM_MAGIC = 0x4C42;  // controle of het EEPROM al geldige parameters bevat

// ---------- parameters in EEPROM ----------
struct param_t
{
  uint16_t magic;
  unsigned long cycleTime;  // cyclustijd in µs
  unsigned long blinkTime;  // knippertijd van de led in ms
  bool debug;               // true = elke seconde de status doorsturen
} params;

// ---------- communicatie: USB (Serial) en bluetooth (Serial1) ----------
TimeoutStream usbStream(Serial, 50);
TimeoutStream btStream(Serial1, 50);
SerialCommand usbCmd(usbStream);
SerialCommand btCmd(btStream);
Stream* out = &Serial;  // poort van het huidige commando: daar gaat het antwoord naartoe

// ---------- toestand ----------
volatile bool running = false;
volatile unsigned long lastPress = 0;
bool wasRunning = false;
unsigned long previous, calculationTime;
unsigned long lastBlink = 0, lastDebug = 0;
unsigned long cycleCount = 0;

// bericht naar USB en bluetooth, enkel als er plaats is in de zendbuffer (niet-blokkerend)
void sendAll(const char* msg)
{
  size_t len = strlen(msg) + 2;
  if (Serial.dtr() && Serial.availableForWrite() >= (int)len) Serial.println(msg);
  if (Serial1.availableForWrite() >= (int)len) Serial1.println(msg);
}

// ---------- interrupt: drukknop ----------
void onStartButton()
{
  unsigned long now = millis();
  if (now - lastPress < DEBOUNCE_MS) return;
  lastPress = now;
  running = !running;
}

void setup()
{
  pinMode(PIN_NFAULT, INPUT);    // de USB-code van de Leonardo gebruikt PD5 als TX-led: terug als ingang
  pinMode(PIN_NSLEEP, OUTPUT);
  digitalWrite(PIN_NSLEEP, LOW); // motoren blijven uit in het basisprogramma
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  pinMode(PIN_START, INPUT);     // externe pull-up van 10k

  Serial.begin(Baudrate);
  Serial1.begin(BtBaudrate);

  addCommands(usbCmd);
  addCommands(btCmd);

  EEPROM_readAnything(0, params);
  if (params.magic != PARAM_MAGIC)  // leeg EEPROM: standaardwaarden
  {
    params.magic = PARAM_MAGIC;
    params.cycleTime = 5000;
    params.blinkTime = 500;
    params.debug = false;
    EEPROM_writeAnything(0, params);
  }

  attachInterrupt(digitalPinToInterrupt(PIN_START), onStartButton, FALLING);

  sendAll("ready");
}

void loop()
{
  // commando's inlezen (niet-blokkerend)
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

// code die cyclisch uitgevoerd wordt
void cycle()
{
  cycleCount++;

  // start / stop melden (knop of commando)
  if (running != wasRunning)
  {
    wasRunning = running;
    sendAll(running ? "gestart" : "gestopt");
    if (!running) digitalWrite(PIN_LED, LOW);
  }

  // voorbeeld: knipperende led zolang de robot gestart is
  if (running && millis() - lastBlink >= params.blinkTime)
  {
    lastBlink = millis();
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
  }

  // debug: elke seconde de status doorsturen
  if (params.debug && millis() - lastDebug >= 1000)
  {
    lastDebug = millis();
    char msg[64];
    snprintf(msg, sizeof(msg), "%s, cycli %lu, rekentijd %lu us", running ? "gestart" : "gestopt", cycleCount, calculationTime);
    sendAll(msg);
  }
}

// ---------- commando's ----------
void addCommands(SerialCommand& cmd)
{
  cmd.addCommand("set", onSet);
  cmd.addCommand("debug", onDebug);
  cmd.addCommand("start", onStart);
  cmd.addCommand("stop", onStop);
  cmd.addCommand("help", onHelp);
  cmd.setDefaultHandler(onUnknownCommand);
}

SerialCommand& currentCmd() { return out == &Serial ? usbCmd : btCmd; }

void onUnknownCommand(char* command)
{
  out->print(F("unknown command: \""));
  out->print(command);
  out->println(F("\""));
}

// set cycle <µs> | set blink <ms> | set debug <on|off>
void onSet()
{
  char* param = currentCmd().next();
  char* value = currentCmd().next();
  if (param == NULL || value == NULL)
  {
    out->println(F("gebruik: set <cycle|blink|debug> <waarde>"));
    return;
  }

  if      (strcmp(param, "cycle") == 0) params.cycleTime = constrain(atol(value), 500, 1000000);
  else if (strcmp(param, "blink") == 0) params.blinkTime = constrain(atol(value), 10, 10000);
  else if (strcmp(param, "debug") == 0) params.debug = (strcmp(value, "on") == 0 || strcmp(value, "1") == 0);
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

// alle parameters en de rekentijd tonen
void onDebug()
{
  out->print(F("cycle time: "));       out->println(params.cycleTime);
  out->print(F("blink time: "));       out->println(params.blinkTime);
  out->print(F("debug: "));            out->println(params.debug ? F("on") : F("off"));
  out->print(F("toestand: "));         out->println(running ? F("gestart") : F("gestopt"));
  out->print(F("calculation time: ")); out->println(calculationTime);
  calculationTime = 0;
}

void onStart()
{
  running = true;
}

void onStop()
{
  running = false;
}

void onHelp()
{
  out->println(F("set cycle <us> | set blink <ms> | set debug <on|off>"));
  out->println(F("start | stop | debug | help"));
}

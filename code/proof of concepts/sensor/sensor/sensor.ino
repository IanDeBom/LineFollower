// proof of concept sensoren (QTR-8A)
// de 8 sensoren worden individueel en ruw uitgelezen (geen calibratie, normalisatie of interpolatie)
// en 10x per seconde doorgestuurd naar de seriële monitor / seriële plotter (9600 baud)
//
// commando's via de seriële monitor (regeleinde Newline):
//   aan    IR-leds van de sensor aan (standaard)
//   uit    IR-leds uit: enkel omgevingslicht wordt gemeten

#include <Arduino.h>
#include <SerialCommands.h>

// QTR-8A lijnsensor, zie aansluitschema
const uint8_t SENSOR_COUNT = 8;
const uint8_t sensorPins[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5, A6, A7};
const uint8_t LEDON = 7;  // hoog = IR-leds aan

const unsigned long PRINT_INTERVAL_MS = 100;

int values[SENSOR_COUNT];

// ---------- commando's ----------
char serialBuffer[32];
SerialCommands serialCommands(&Serial, serialBuffer, sizeof(serialBuffer), "\n", " \r");

void cmdAan(SerialCommands* sender)
{
  digitalWrite(LEDON, HIGH);
}

void cmdUit(SerialCommands* sender)
{
  digitalWrite(LEDON, LOW);
}

SerialCommand serialAan("aan", cmdAan);
SerialCommand serialUit("uit", cmdUit);

// ---------- sensoren ----------
void readSensors()
{
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    values[i] = analogRead(sensorPins[i]);  // 0 (wit, veel reflectie) .. 1023 (zwart, geen reflectie)
  }
}

// formaat "s1:512 s2:498 ..." wordt zowel in de seriële monitor als in de seriële plotter leesbaar getoond
void printSensors()
{
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print('s');
    Serial.print(i + 1);
    Serial.print(':');
    Serial.print(values[i]);
    Serial.print(i < SENSOR_COUNT - 1 ? ' ' : '\n');
  }
}

void setup()
{
  pinMode(LEDON, OUTPUT);
  digitalWrite(LEDON, HIGH);  // IR-leds aan

  analogReference(DEFAULT);   // referentie = 5 V = voedingsspanning van de sensor: volledig bereik 0..1023

  Serial.begin(9600);
  serialCommands.AddCommand(&serialAan);
  serialCommands.AddCommand(&serialUit);
}

void loop()
{
  static unsigned long lastPrint = 0;

  serialCommands.ReadSerial();

  if (millis() - lastPrint >= PRINT_INTERVAL_MS)
  {
    lastPrint = millis();
    readSensors();
    printSensors();
  }
}

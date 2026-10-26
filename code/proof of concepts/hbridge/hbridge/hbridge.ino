// proof of concept H-brug (DRV8833)
// 2 motoren onafhankelijk, traploos in snelheid en in beide richtingen
// de motoren worden gesimuleerd met 2 antiparallelle leds per uitgang:
// led "vooruit" licht op bij vooruit, led "achteruit" bij achteruit, helderheid = snelheid
//
// commando's via de seriële monitor (9600 baud, regeleinde Newline):
//   a <snelheid>   motor A instellen, -255 (achteruit) .. 0 (stil) .. 255 (vooruit)
//   b <snelheid>   motor B instellen
//   demo           automatische demo (standaard bij opstarten)
//   stop           beide motoren stil, demo uit

#include <Arduino.h>
#include <SerialCommands.h>

// DRV8833 motordriver, zie aansluitschema
const uint8_t AIN1   = 5;   // motor A, PWM (timer 0)
const uint8_t AIN2   = 6;   // motor A, PWM (timer 0)
const uint8_t BIN1   = 9;   // motor B, PWM (timer 1)
const uint8_t BIN2   = 10;  // motor B, PWM (timer 1)
const uint8_t STBY   = 8;   // hoog = driver aan (nSLEEP van de DRV8833)

const unsigned long DEMO_STEP_MS = 20;

bool demo = true;
int speedA = 0;
int speedB = 0;

// snelheid -255 .. 255: PWM op IN1 = vooruit, PWM op IN2 = achteruit, 0 = uitbollen
void setMotor(uint8_t in1, uint8_t in2, int speed)
{
  speed = constrain(speed, -255, 255);
  if (speed >= 0)
  {
    analogWrite(in1, speed);
    digitalWrite(in2, LOW);
  }
  else
  {
    digitalWrite(in1, LOW);
    analogWrite(in2, -speed);
  }
}

void setMotors(int a, int b)
{
  speedA = constrain(a, -255, 255);
  speedB = constrain(b, -255, 255);
  setMotor(AIN1, AIN2, speedA);
  setMotor(BIN1, BIN2, speedB);
}

void printSpeeds()
{
  Serial.print("A: ");
  Serial.print(speedA);
  Serial.print("\tB: ");
  Serial.println(speedB);
}

// ---------- commando's ----------
char serialBuffer[32];
SerialCommands serialCommands(&Serial, serialBuffer, sizeof(serialBuffer), "\n", " \r");

void cmdA(SerialCommands* sender)
{
  char* arg = sender->Next();
  if (arg == NULL) return;
  demo = false;
  setMotors(atoi(arg), speedB);
  printSpeeds();
}

void cmdB(SerialCommands* sender)
{
  char* arg = sender->Next();
  if (arg == NULL) return;
  demo = false;
  setMotors(speedA, atoi(arg));
  printSpeeds();
}

void cmdDemo(SerialCommands* sender)
{
  demo = true;
  Serial.println("demo aan");
}

void cmdStop(SerialCommands* sender)
{
  demo = false;
  setMotors(0, 0);
  printSpeeds();
}

void cmdOnbekend(SerialCommands* sender, const char* cmd)
{
  Serial.print("onbekend: ");
  Serial.println(cmd);
  Serial.println("gebruik: a <-255..255>, b <-255..255>, demo, stop");
}

SerialCommand serialA("a", cmdA);
SerialCommand serialB("b", cmdB);
SerialCommand serialDemo("demo", cmdDemo);
SerialCommand serialStop("stop", cmdStop);

// ---------- demo ----------
// motor A en B lopen elk met een andere snelheid op en af tussen -255 en 255,
// zo is te zien dat ze onafhankelijk van elkaar geregeld worden
void runDemo()
{
  static unsigned long lastStep = 0;
  static int stepA = 5;
  static int stepB = -3;

  if (millis() - lastStep < DEMO_STEP_MS) return;
  lastStep = millis();

  int a = speedA + stepA;
  int b = speedB + stepB;
  if (a >= 255 || a <= -255) stepA = -stepA;
  if (b >= 255 || b <= -255) stepB = -stepB;
  setMotors(a, b);
}

void setup()
{
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  setMotors(0, 0);
  digitalWrite(STBY, HIGH);  // driver aan

  Serial.begin(9600);
  serialCommands.SetDefaultHandler(cmdOnbekend);
  serialCommands.AddCommand(&serialA);
  serialCommands.AddCommand(&serialB);
  serialCommands.AddCommand(&serialDemo);
  serialCommands.AddCommand(&serialStop);

  Serial.println("proof of concept H-brug gestart (demo)");
  Serial.println("gebruik: a <-255..255>, b <-255..255>, demo, stop");
}

void loop()
{
  serialCommands.ReadSerial();
  if (demo) runDemo();
}

// proof of concept start/stop interrupt
// drukknop op D2 (INT0) schakelt de led op D4 onmiddellijk tussen gestart / gestopt,
// ook al zit de loop vast in een trage, blokkerende taak (knipperen van de led op D13 met delay)

#include <Arduino.h>

const uint8_t BUTTON = 2;   // INT0, drukknop naar GND (interne pull-up)
const uint8_t LED    = 4;   // status: aan = gestart, uit = gestopt (560 ohm naar GND)
const uint8_t BUSY   = 13;  // ingebouwde led, simuleert de rest van het programma

const unsigned long DEBOUNCE_MS = 200;  // minimale tijd tussen twee drukken

volatile bool running = false;           // gedeeld met de interrupt, dus volatile
volatile unsigned long lastPress = 0;

// interrupt service routine: zo kort mogelijk houden
void onButton()
{
  unsigned long now = millis();
  if (now - lastPress < DEBOUNCE_MS) return;  // dender van de knop negeren
  if (digitalRead(BUTTON) == HIGH) return;    // stoorpuls of dender bij het loslaten
  lastPress = now;

  running = !running;
  digitalWrite(LED, running);  // led reageert direct, niet pas in de loop
}

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(BUSY, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(BUTTON), onButton, FALLING);  // hoog -> laag = ingedrukt

  Serial.println("proof of concept start/stop interrupt gestart");
}

void loop()
{
  static bool lastRunning = false;

  // trage, blokkerende taak: 2 s per cyclus, toch reageert de knop direct
  digitalWrite(BUSY, HIGH);
  delay(1000);
  digitalWrite(BUSY, LOW);
  delay(1000);

  // toestand kopiëren zonder dat de interrupt er tussenkomt
  noInterrupts();
  bool nowRunning = running;
  interrupts();

  if (nowRunning != lastRunning)
  {
    Serial.println(nowRunning ? "gestart" : "gestopt");
    lastRunning = nowRunning;
  }
}

// proof of concept draadloze communicatie
// led op D4 aan/uit schakelen met commando's via de BT05 bluetooth (BLE) module
// commando's: aan, uit, status (met of zonder enter)

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SerialCommands.h>

// BT05 bluetooth (SoftwareSerial), zie aansluitschema
const uint8_t BT_RX = 11;  // naar TXD van BT05
const uint8_t BT_TX = 12;  // naar RXD van BT05 (via spanningsdeler 1k / 2k)
const uint8_t LED   = 4;   // led met 560 ohm naar GND

SoftwareSerial bluetooth(BT_RX, BT_TX);

// BLE-apps (bv. LightBlue op iPhone) sturen een commando meestal zonder enter.
// Deze stream geeft een extra "\r" door als er een tijd niets meer binnenkomt,
// zodat SerialCommands het commando toch uitvoert.
class TimeoutStream : public Stream
{
public:
  TimeoutStream(Stream& stream, unsigned long timeout) : stream_(stream), timeout_(timeout) {}

  int available() override { return stream_.available() ? stream_.available() : (timedOut() ? 1 : 0); }
  int peek() override { return stream_.available() ? stream_.peek() : (timedOut() ? '\r' : -1); }
  int read() override
  {
    if (stream_.available())
    {
      int c = stream_.read();
      pending_ = (c != '\r' && c != '\n');
      lastRead_ = millis();
      return c;
    }
    if (timedOut())
    {
      pending_ = false;
      return '\r';
    }
    return -1;
  }
  size_t write(uint8_t c) override { return stream_.write(c); }

private:
  bool timedOut() { return pending_ && millis() - lastRead_ >= timeout_; }

  Stream& stream_;
  unsigned long timeout_;
  unsigned long lastRead_ = 0;
  bool pending_ = false;
};

TimeoutStream bluetoothStream(bluetooth, 50);

// commando's eindigen op "\r" (of 50 ms stilte), "\n" wordt mee als scheidingsteken gezien
char serialBuffer[32];
SerialCommands serialCommands(&bluetoothStream, serialBuffer, sizeof(serialBuffer), "\r", " \n");

void printStatus(Stream* serial)
{
  serial->print("led: ");
  serial->println(digitalRead(LED) ? "aan" : "uit");
}

void cmdAan(SerialCommands* sender)
{
  digitalWrite(LED, HIGH);
  printStatus(sender->GetSerial());
}

void cmdUit(SerialCommands* sender)
{
  digitalWrite(LED, LOW);
  printStatus(sender->GetSerial());
}

void cmdStatus(SerialCommands* sender)
{
  printStatus(sender->GetSerial());
}

void cmdOnbekend(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("onbekend: ");
  sender->GetSerial()->println(cmd);
}

SerialCommand serialAan("aan", cmdAan);
SerialCommand serialUit("uit", cmdUit);
SerialCommand serialStatus("status", cmdStatus);

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);     // USB, voor debug
  bluetooth.begin(9600);  // standaard baudrate BT05

  serialCommands.SetDefaultHandler(cmdOnbekend);
  serialCommands.AddCommand(&serialAan);
  serialCommands.AddCommand(&serialUit);
  serialCommands.AddCommand(&serialStatus);

  Serial.println("proof of concept draadloze communicatie gestart");
}

void loop()
{
  serialCommands.ReadSerial();
}

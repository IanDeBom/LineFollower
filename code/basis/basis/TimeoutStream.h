#ifndef TimeoutStream_h
#define TimeoutStream_h

#include <Arduino.h>

// Stream-wrapper voor SerialCommand (die een commando pas uitvoert na een '\r'):
// - '\n' wordt doorgegeven als '\r', zodat ook terminals met enkel Newline werken
// - komt er na een commando een tijd niets meer binnen, dan wordt er zelf een '\r' toegevoegd,
//   zodat BLE-apps (bv. LightBlue op iPhone) die zonder enter sturen ook werken
class TimeoutStream : public Stream
{
public:
  TimeoutStream(Stream& stream, unsigned long timeout) : stream_(stream), timeout_(timeout) {}

  int available() override { return stream_.available() ? stream_.available() : (timedOut() ? 1 : 0); }
  int peek() override { return stream_.available() ? convert(stream_.peek()) : (timedOut() ? '\r' : -1); }
  int read() override
  {
    if (stream_.available())
    {
      int c = convert(stream_.read());
      pending_ = (c != '\r');
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
  int availableForWrite() override { return stream_.availableForWrite(); }

private:
  static int convert(int c) { return c == '\n' ? '\r' : c; }
  bool timedOut() { return pending_ && millis() - lastRead_ >= timeout_; }

  Stream& stream_;
  unsigned long timeout_;
  unsigned long lastRead_ = 0;
  bool pending_ = false;
};

#endif

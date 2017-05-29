#ifndef NTP_HPP
#define  NTP_HPP

#include "Arduino.h"
#include "WiFiUdp.h"

#define NTP_DEBUG 1
#define SERVER_POOL_URL "br.pool.ntp.org"
#define NTP_PACKAGE_SIZE 48

class Ntp
{
public:
  Ntp(int8_t offset=0, int32_t updateInterval=10000, const uint16_t port=2390);
  Ntp(const char* server, int8_t offset=0,\
     int32_t updateInterval=10000, const uint16_t port=2390);

  bool begin();   
  void getServerHost();
  uint32_t secondsSince1900();

private:
  const uint32_t seventyYears = 2208988800UL;
  const uint8_t _port;
  const char* _server;
  byte _packageBuffer[NTP_PACKAGE_SIZE];
  IPAddress _timeServerIP;
  WiFiUDP _udp;

  bool sendNTPpackage();
};

#endif

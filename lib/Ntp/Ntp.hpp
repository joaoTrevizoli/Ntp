/*Ntp
    __  _       _      ___   ___  _  _    __
  / / | | __ _| |__  ( _ ) / _ \| || |   \ \
/ /   | |/ _` | '_ \ / _ \| | | | || |_   \ \
\ \   | | (_| | |_) | (_) | |_| |__   _|  / /
 \_\  |_|\__,_|_.__/ \___/ \___/   |_|   /_/

Interface of the Network Time Protocol

Created by: João Trevizoli Esteves
*/

#ifndef NTP_HPP
#define  NTP_HPP

#include "Arduino.h"
#include "WiFiUdp.h"
#include "ESP8266WiFi.h"

#define NTP_DEBUG 1

#define SEVENTY_YEARS 2208988800UL

// -------------------------NTP packet config ------------------------------- //

#define SERVER_POOL_URL "br.pool.ntp.org"
#define NTP_PACKAGE_SIZE 48
#define NTP_LOCAL_PORT 2390
#define NTP_HOST_PORT 123
#define LI 0b11
#define NTP_VERSION 0b100
#define NTP_PACKET_MODE 0b011
#define STRATUM_0 0
#define STRATUM_1 1
#define STRATUM_2 2
#define STRATUM_3 3
#define NTP_POOL 6
#define PEER_CLOCK_PRECISION 0XEC

// -------------------------Class Interface---------------------------------- //

class Ntp
{
public:
  int8_t _utcOffset;
  WiFiUDP* _udp;

  Ntp(WiFiUDP& udp, int8_t utcOffset=0,\
     uint32_t updateInterval=10000, \
     const uint16_t port=NTP_LOCAL_PORT);

  Ntp(WiFiUDP& udp, const char* server,\
     int8_t utcOffset=0,\
     int32_t updateInterval=10000,\
     const uint16_t port=NTP_LOCAL_PORT);

  bool begin();
  void getServerHost();
  void resetUtcOffset(int8_t utcOffset);
  uint32_t secondsSince1900(bool force=false);
  uint32_t unixTimeStamp(bool force=false);


private:

  uint32_t _updateInterval;
  uint32_t _lastUpdate = 0;
  uint32_t _secondsSince1900 = 0;
  const uint16_t _port;
  const char* _server = SERVER_POOL_URL;


  byte _packageBuffer[NTP_PACKAGE_SIZE];
  IPAddress _timeServerIP;

  bool getNewHostIP();
  void formatPackage();
  bool sendNTPpackage();
  bool update();
  bool updateForce();

};

#endif

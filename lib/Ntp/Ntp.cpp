/*Ntp
    __  _       _      ___   ___  _  _    __
  / / | | __ _| |__  ( _ ) / _ \| || |   \ \
/ /   | |/ _` | '_ \ / _ \| | | | || |_   \ \
\ \   | | (_| | |_) | (_) | |_| |__   _|  / /
 \_\  |_|\__,_|_.__/ \___/ \___/   |_|   /_/

Implementation of the Network Time Protocol

Created by: João Trevizoli Esteves
*/

#include "Ntp.hpp"

// -------------------------Constructors------------------------------------- //

Ntp::Ntp(WiFiUDP& udp, int8_t offset, uint32_t updateInterval, \
  const uint16_t port):
  _port(port)
{
  this->_udp = &udp;
  this->_offset = offset;
  this->_updateInterval = updateInterval;
}

// -------------------------------------------------------------------------- //

Ntp::Ntp(WiFiUDP& udp, const char* server, int8_t offset,\
   int32_t updateInterval, const uint16_t port):
   _server(server),
   _port(port)
{
  this->_udp = &udp;
  this->_offset = offset;
  this->_updateInterval = updateInterval;
}

// -------------------------Public methods----------------------------------- //

bool Ntp::begin()
{
  Serial.println(this->_server);
  Serial.println(this->_updateInterval);
  Serial.println(this->_port);
  Serial.println(STRATUM_0);

  this->_udp->begin(this->_port);
  this->getNewHostIP();
}

// -------------------------------------------------------------------------- //

void Ntp::getServerHost()
{
  Serial.println(this->_server);
  Serial.print("Host ip: ");
  Serial.println(this->_timeServerIP);
}

// -------------------------------------------------------------------------- //

uint32_t Ntp::secondsSince1900(bool force)
{
  if(!force) this->update();
  else this->updateForce();

  return this->_secondsSince1900;
}

// -------------------------------------------------------------------------- //

uint32_t Ntp::unixTimeStamp(bool force)
{
  if(!force) this->update();
  else this->updateForce();

  if(this->_secondsSince1900 != 0)
  {
    return this->_secondsSince1900 - SEVENTY_YEARS;
  }
  else
  {
    return 0;
  }
}

// -------------------------Private methods---------------------------------- //

bool Ntp::getNewHostIP()
{
   #if NTP_DEBUG == 1
      Serial.println("Getting new ip from server pool...");
   #endif

   if (WiFi.status() == WL_CONNECTED)
   {
     WiFi.hostByName(this->_server, this->_timeServerIP);
     #if NTP_DEBUG == 1
      Serial.print("host ip: ");
      Serial.println(this->_timeServerIP);
     #endif
     return true;
   }
   else
   {
     #if NTP_DEBUG == 1
      Serial.println("Failed to get new ip, WiFi disconnected");
     #endif
     return false;
   }
}

// -------------------------------------------------------------------------- //

void Ntp::formatPackage()
{
  memset(this->_packageBuffer, 0, NTP_PACKAGE_SIZE);
  this->_packageBuffer[0] = (LI << 3 | NTP_VERSION) << 3 | NTP_PACKET_MODE;
  this->_packageBuffer[1] = STRATUM_0;
  this->_packageBuffer[2] = NTP_POOL;
  this->_packageBuffer[3] = PEER_CLOCK_PRECISION;
  this->_packageBuffer[12]  = 49;
  this->_packageBuffer[13]  = 0x4E;
  this->_packageBuffer[14]  = 49;
  this->_packageBuffer[15]  = 52;
}

// -------------------------------------------------------------------------- //

bool Ntp::sendNTPpackage()
{
  this->formatPackage();

  #if NTP_DEBUG == 1
    Serial.println("sending NTP packet...");
  #endif
  this->_udp->beginPacket(this->_timeServerIP, NTP_HOST_PORT);
  this->_udp->write(this->_packageBuffer, NTP_PACKAGE_SIZE);
  this->_udp->endPacket();
}

// -------------------------------------------------------------------------- //

bool Ntp::update()
{
  if ((millis() - this->_lastUpdate >= this->_updateInterval) \
  || this->_lastUpdate == 0
  || this->_secondsSince1900 == 0)
  {
    this->getNewHostIP();
    this->_lastUpdate = millis();
    return this->updateForce();
  }
  return true;
}

// -------------------------------------------------------------------------- //

bool Ntp::updateForce()
{
   this->sendNTPpackage();
   byte timeOut = 0;
   int8_t cb = 0;
   do
   {
     delay(10);
     cb = this->_udp->parsePacket();
     if(timeOut > 100)
     {
      Serial.println("TimeOut, uneble to receive an package...");
      return false;
     }
     timeOut++;
   }while (cb==0);

   this->_udp->read(this->_packageBuffer, NTP_PACKAGE_SIZE);
   unsigned long highWord = word(this->_packageBuffer[40], this->_packageBuffer[41]);
   unsigned long lowWord = word(this->_packageBuffer[42], this->_packageBuffer[43]);
   this->_secondsSince1900 = highWord << 16 | lowWord;
   return true;
}
// -------------------------------------------------------------------------- //

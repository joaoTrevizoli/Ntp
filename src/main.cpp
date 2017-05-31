#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include "Ntp.hpp"

char ssid[] = "__";
char pass[] =  "__";

WiFiUDP udp;
Ntp ntpClient(udp, 10, 500);

void setup()
{
    Serial.begin(115200);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    ntpClient.begin();

}

void loop()
{
  Serial.println(ntpClient.secondsSince1900());
  Serial.println(ntpClient.unixTimeStamp());

  delay(500);
}

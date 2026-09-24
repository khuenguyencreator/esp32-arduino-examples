#include <Arduino.h>
#include <HTTPUpdate.h>

void update_FOTA();
void setup()
{
  Serial.begin(115200);
  WiFi.begin("Ten_Wifi_Cua_Ban", "Mat_Khau_Wifi");
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

uint32_t updateCounter = 0;
String version = "1.1";
String key = "OTAdrive_API_Key_Cua_Ban"; // lay tai muc Product cua otadrive.com

void loop()
{
  Serial.print("Ver: ");
  Serial.println(version);
  delay(500);
  if (WiFi.status() == WL_CONNECTED)
  {
    updateCounter++;
    if (updateCounter > 10)
    {
      updateCounter = 0;
      Serial.println("Check update");
      update_FOTA();
    }
  }
}

String getChipId()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}

void update_FOTA()
{
  String url = "http://otadrive.com/deviceapi/update?";
  url += "k=" + key;
  url += "&v=" + version;
  url += "&s=" + getChipId(); // định danh thiết bị trên Cloud

  WiFiClient client;
  httpUpdate.update(client, url, version);
}
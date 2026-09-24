#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <string.h>
const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

String weatherApiKey = "Openweather_API_Key_Cua_Ban"; // dang ky mien phi tai openweathermap.org

String city = "Hanoi";
String countrycode = "";
String weatherJsonBuffer;

//http config
String weatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + weatherApiKey;

int temp = 30;
int humi = 80;

String httpGETRequest(const char* Url);
void setup() {
  Serial.begin(115200);

  //setup wifi
  WiFi.begin(ssid,password);
  Serial.println("conecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //read temp to openweather
  Serial.println(weatherUrl);
  weatherJsonBuffer = httpGETRequest(weatherUrl.c_str());
  Serial.println(weatherJsonBuffer);
  JSONVar weatherJson = JSON.parse(weatherJsonBuffer);
  if(JSON.typeof_(weatherJson) == "undefined")
  {
    Serial.println("Parsing fail");
    return;
  }
  Serial.println();
  temp = weatherJson["main"]["temp"];
  temp = temp - 273;
  Serial.print("Temperature: ");
  Serial.println(temp);

  humi = weatherJson["main"]["humidity"];
  Serial.print("Humidity: ");
  Serial.println(humi);
  delay(10000);
}

String httpGETRequest(const char* Url)
{
  HTTPClient http;
  http.begin(Url);
  int responseCode = http.GET();
  String responseBody = "{}";
  if(responseCode > 0)
  {
    Serial.print("responseCode:");
    Serial.println(responseCode);
    responseBody = http.getString();
  }
  else 
  {
    Serial.print("Error Code: ");
    Serial.println(responseCode);
  }
  http.end();
  return responseBody;
}

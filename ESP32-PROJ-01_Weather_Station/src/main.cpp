#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Du an tong hop: ket hop lai kien thuc tu bai HTTPS (goi API) va bai OLED
// (hien thi man hinh) truoc do - thay vi chi in du lieu thoi tiet ra Serial
// Monitor, du an nay hien thi truc tiep len man hinh OLED, thanh 1 tram du
// bao thoi tiet mini de ban co the dat tren ban lam viec.

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

const char* city = "Hanoi";
const char* apiKey = "Openweather_API_Key_Cua_Ban"; // dang ky mien phi tai openweathermap.org

void updateWeather() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  String url = "https://api.openweathermap.org/data/2.5/weather?q=" + String(city) +
               "&appid=" + String(apiKey) + "&units=metric&lang=vi";

  display.clearDisplay();
  display.setCursor(0, 0);

  if (https.begin(client, url)) {
    int httpCode = https.GET();

    if (httpCode == 200) {
      String payload = https.getString();

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      float temp = doc["main"]["temp"];
      float humidity = doc["main"]["humidity"];
      String description = doc["weather"][0]["description"].as<String>();

      display.setTextSize(1);
      display.println(city);
      display.println();

      display.setTextSize(2);
      display.print(temp, 1);
      display.println(" C");

      display.setTextSize(1);
      display.print("Do am: ");
      display.print(humidity);
      display.println("%");
      display.println(description);

      Serial.println("Cap nhat thoi tiet thanh cong");
    } else {
      display.println("Loi goi API thoi tiet");
      Serial.print("Ma loi HTTP: ");
      Serial.println(httpCode);
    }
    https.end();
  } else {
    display.println("Khong ket noi duoc API");
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Khong tim thay man hinh OLED");
    while (true) delay(1000);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Dang ket noi WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    updateWeather();
  }

  delay(600000); // cap nhat moi 10 phut, tranh vuot han muc goi API mien phi
}

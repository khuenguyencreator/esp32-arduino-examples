#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Firebase Realtime Database cung cap san 1 REST API qua HTTPS - khong can
// cai thu vien Firebase rieng, chi can HTTPClient nhu cac bai HTTPS/API
// truoc la du de doc/ghi du lieu, ghi gia tri cam bien len "dam may" theo
// thoi gian thuc.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

// URL goc cua Realtime Database, lay trong Firebase Console > Realtime Database
// Vi du: https://ten-du-an-cua-ban-default-rtdb.asia-southeast1.firebasedatabase.app
const char* firebaseHost = "https://ten-du-an-cua-ban-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* firebaseAuth = "Database_Secret_Cua_Ban"; // lay trong Project Settings > Service Accounts > Database secrets

void sendTemperature(float temperature) {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  String url = String(firebaseHost) + "/sensor/temperature.json?auth=" + firebaseAuth;

  if (https.begin(client, url)) {
    https.addHeader("Content-Type", "application/json");
    String payload = String(temperature);

    int httpCode = https.PUT(payload);

    if (httpCode > 0) {
      Serial.print("Ma phan hoi Firebase: ");
      Serial.println(httpCode);
    } else {
      Serial.print("Loi khi goi Firebase: ");
      Serial.println(https.errorToString(httpCode));
    }
    https.end();
  } else {
    Serial.println("Khong the ket noi toi Firebase");
  }
}

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = random(200, 350) / 10.0; // gia lap cam bien nhiet do

    Serial.print("Dang gui nhiet do len Firebase: ");
    Serial.println(temperature);
    sendTemperature(temperature);
  }

  delay(10000);
}

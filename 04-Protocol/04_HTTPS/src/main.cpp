#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Bai nay minh hoa cach ESP32 goi 1 API cong khai qua HTTPS (co ma hoa TLS),
// khac voi cac bai truoc chi dung HTTP thuong (khong ma hoa).

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

// API test cong khai, khong can dang ky key, luon tra ve 1 JSON co dinh de test
const char* apiUrl = "https://jsonplaceholder.typicode.com/todos/1";

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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    // Bo qua kiem tra chung chi SSL cho don gian (chi nen dung khi hoc/test,
    // khong nen dung cho san pham thuc te - xem Luu y trong bai viet)
    client.setInsecure();

    HTTPClient https;
    Serial.println("Dang goi API qua HTTPS...");

    if (https.begin(client, apiUrl)) {
      int httpCode = https.GET();

      if (httpCode > 0) {
        Serial.print("Ma phan hoi: ");
        Serial.println(httpCode);
        String payload = https.getString();
        Serial.println("Noi dung tra ve:");
        Serial.println(payload);
      } else {
        Serial.print("Loi khi goi HTTPS: ");
        Serial.println(https.errorToString(httpCode));
      }
      https.end();
    } else {
      Serial.println("Khong the ket noi toi server HTTPS");
    }
  }

  delay(10000); // goi lai sau moi 10 giay
}

#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>

// Node-RED chay tren may tinh/Raspberry Pi trong cung mang LAN, mo san 1
// endpoint HTTP (node "http in") de nhan du lieu - ESP32 chi can gui POST
// request kem JSON len dung endpoint do, Node-RED se nhan va xu ly/hien thi
// tren dashboard theo cach ban tu keo-tha cau hinh.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

// Dia chi IP cua may chay Node-RED trong mang LAN, endpoint dat ten "/esp32data"
const char* nodeRedUrl = "http://192.168.1.100:1880/esp32data";

void sendToNodeRED(float temperature, float humidity) {
  HTTPClient http;

  if (http.begin(nodeRedUrl)) {
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"temperature\":" + String(temperature) +
                      ",\"humidity\":" + String(humidity) + "}";

    int httpCode = http.POST(payload);

    if (httpCode > 0) {
      Serial.print("Ma phan hoi Node-RED: ");
      Serial.println(httpCode);
      Serial.println(http.getString());
    } else {
      Serial.print("Loi khi goi Node-RED: ");
      Serial.println(http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println("Khong the ket noi toi Node-RED - kiem tra lai dia chi IP");
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
    float temperature = random(200, 350) / 10.0;
    float humidity = random(400, 800) / 10.0;

    Serial.println("Dang gui du lieu len Node-RED...");
    sendToNodeRED(temperature, humidity);
  }

  delay(5000);
}

#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Bai nay minh hoa cach ESP32 phuc vu 1 trang web duoc luu san trong SPIFFS
// (bo nho flash con lai sau khi tru phan chua firmware), thay vi nhung ca
// chuoi HTML vao trong code C++ nhu cac bai Webserver truoc.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

WebServer server(80);

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    server.send(404, "text/plain", "Khong tim thay file index.html trong SPIFFS");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void handleNotFound() {
  server.send(404, "text/plain", "404 - Khong tim thay trang");
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("Loi: khong mount duoc SPIFFS");
    return;
  }
  Serial.println("Mount SPIFFS thanh cong");

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

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server da khoi dong");
}

void loop() {
  server.handleClient();
}

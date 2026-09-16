#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Bai nay minh hoa cach luu du lieu BEN VUNG (khong mat khi cup dien/reset)
// vao SPIFFS: moi lan co nguoi truy cap trang web, ESP32 doc so dem cu tu file
// trong SPIFFS, tang len 1, roi ghi lai vao chinh file do.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";
const char* counterFile = "/counter.txt";

WebServer server(80);

int readCounter() {
  if (!SPIFFS.exists(counterFile)) {
    return 0; // chua co file nghia la chua truy cap lan nao
  }
  File file = SPIFFS.open(counterFile, "r");
  int value = file.parseInt();
  file.close();
  return value;
}

void writeCounter(int value) {
  File file = SPIFFS.open(counterFile, "w"); // mo kieu "w" se ghi de toan bo file cu
  file.print(value);
  file.close();
}

void handleRoot() {
  int count = readCounter() + 1;
  writeCounter(count);

  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} h1{color:Tomato;}</style></head>";
  html += "<body><h1>ESP32 SPIFFS Data Storage</h1>";
  html += "<p>So lan trang nay duoc truy cap: <b>" + String(count) + "</b></p>";
  html += "<p>Thu tat nguon ESP32 roi cap lai, refresh trang - so dem van tiep tuc tang chu khong ve lai 0.</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleReset() {
  writeCounter(0);
  server.sendHeader("Location", "/");
  server.send(303); // redirect ve trang chu sau khi reset
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("Loi: khong mount duoc SPIFFS");
    return;
  }
  Serial.println("Mount SPIFFS thanh cong");
  Serial.print("So dem hien tai dang luu trong SPIFFS: ");
  Serial.println(readCounter());

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
  server.on("/reset", handleReset);
  server.begin();
  Serial.println("HTTP server da khoi dong");
}

void loop() {
  server.handleClient();
}

#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// 3 van de thuc te gap o moi bai webserver truoc:
//  1. Phai mo Serial Monitor xem IP moi truy cap duoc
//     -> mDNS: vao thang http://esp32.local, khong can nho IP
//  2. IP do router cap (DHCP) co the doi sau moi lan khoi dong
//     -> IP tinh: tu dat 1 dia chi co dinh cho ESP32
//  3. Router khoi dong lai / mat song WiFi thi ESP32 "chet" luon
//     -> bat su kien WiFi (WiFi events) va tu ket noi lai
//
// Mo trinh duyet vao http://esp32.local (hoac IP tinh ben duoi) de xem
// trang thai. Thu rut dien router roi cam lai de thay ESP32 tu ket noi lai.
//
// Luu y: mDNS chay tot tren Windows 10+, macOS, iOS, Linux. Mot so dien thoai
// Android cu khong phan giai duoc ten .local, khi do dung IP.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";
const char* hostname = "esp32";

// IP tinh - sua cho dung dai mang nha ban (xem IP cua may tinh bang lenh
// ipconfig/ifconfig). Chon dia chi nam ngoai dai DHCP cua router de khong
// bi trung voi thiet bi khac.
const bool USE_STATIC_IP = true;
IPAddress localIP(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

WebServer server(80);

// Cac bien nay duoc sua trong ham xu ly su kien (chay o task WiFi khac voi
// loop), nen khai bao volatile
volatile bool wifiConnected = false;
volatile bool needMdnsRestart = false;
volatile int reconnectCount = 0;
unsigned long lastReconnectAttempt = 0;

void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("[WiFi] Da co IP: ");
      Serial.println(WiFi.localIP());
      wifiConnected = true;
      needMdnsRestart = true;
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      // Chi dat co, KHONG goi ham nang hay delay() trong callback su kien
      if (wifiConnected) {
        Serial.printf("[WiFi] Mat ket noi, ma ly do: %d\n", info.wifi_sta_disconnected.reason);
      }
      wifiConnected = false;
      break;

    default:
      break;
  }
}

void startMdns() {
  MDNS.end();
  if (MDNS.begin(hostname)) {
    MDNS.addService("http", "tcp", 80);   // de cac app "quet mang" thay dich vu web
    Serial.printf("[mDNS] Truy cap: http://%s.local\n", hostname);
  } else {
    Serial.println("[mDNS] Khoi dong that bai");
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<meta http-equiv=\"refresh\" content=\"5\">";   // tu tai lai moi 5 giay
  html += "<style>body{font-family:Helvetica;text-align:center}</style></head><body>";
  html += "<h1>ESP32 Network Status</h1>";
  html += "<p>Ten mDNS: <b>" + String(hostname) + ".local</b></p>";
  html += "<p>IP: <b>" + WiFi.localIP().toString() + "</b> (" + (USE_STATIC_IP ? "tinh" : "DHCP") + ")</p>";
  html += "<p>Cuong do song: <b>" + String(WiFi.RSSI()) + " dBm</b></p>";
  html += "<p>So lan ket noi lai: <b>" + String(reconnectCount) + "</b></p>";
  html += "<p>Thoi gian chay: <b>" + String(millis() / 1000) + " s</b></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname);        // ten hien trong danh sach thiet bi cua router
  WiFi.onEvent(onWiFiEvent);
  WiFi.setAutoReconnect(false);      // tu xu ly ket noi lai trong loop() de de theo doi

  if (USE_STATIC_IP && !WiFi.config(localIP, gateway, subnet, dns)) {
    Serial.println("Cau hinh IP tinh that bai, dung DHCP");
  }

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (!wifiConnected) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  if (wifiConnected) {
    if (needMdnsRestart) {
      needMdnsRestart = false;
      startMdns();   // mDNS phai khoi dong lai sau moi lan co IP moi
    }
    server.handleClient();
  } else if (millis() - lastReconnectAttempt > 5000) {
    // Moi 5 giay thu ket noi lai 1 lan, khong chan loop() nen cac viec
    // khac (doc cam bien, dieu khien...) van chay binh thuong khi mat mang
    lastReconnectAttempt = millis();
    reconnectCount++;
    Serial.printf("[WiFi] Thu ket noi lai lan %d...\n", reconnectCount);
    WiFi.disconnect();
    WiFi.begin(ssid, password);
  }
}

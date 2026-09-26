#include <Arduino.h>

#include <WiFiManager.h>
#include <time.h>

// Bai nay giai quyet 1 van de rat thuc te: tat ca cac bai truoc deu hardcode
// SSID/password ngay trong code - moi lan doi wifi (mang lam, mang nha rieng,
// mang khach) deu phai sua code, nap lai. WiFiManager giai quyet van de nay
// bang "captive portal": ESP32 tu tao 1 mang WiFi rieng, ban dung dien thoai
// ket noi vao va chon wifi that qua giao dien web, khong can sua code.

const char* ntpServer = "pool.ntp.org";
const long gmtOffsetSec = 7 * 3600; // GMT+7 (Viet Nam)
const int daylightOffsetSec = 0;

void syncTimeNTP() {
  configTime(gmtOffsetSec, daylightOffsetSec, ntpServer);

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "Thoi gian hien tai: %A, %d/%m/%Y %H:%M:%S");
  } else {
    Serial.println("Dong bo thoi gian NTP that bai");
  }
}

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;

  // Neu chua tung luu wifi nao (hoac wifi cu khong ket noi duoc), ESP32 se
  // tu tao 1 access point ten "ESP32-Setup" - ket noi dien thoai vao mang
  // nay, trinh duyet se tu mo trang chon wifi that (hoac vao 192.168.4.1)
  bool connected = wifiManager.autoConnect("ESP32-Setup");

  if (!connected) {
    Serial.println("Khong ket noi duoc wifi va het thoi gian cho - khoi dong lai");
    ESP.restart();
  }

  Serial.println("Da ket noi WiFi thanh cong!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  syncTimeNTP();
}

void loop() {
  static unsigned long lastSync = 0;

  // Dong bo lai gio moi 1 gio, phong truong hop dong ho noi bo bi lech dan
  if (millis() - lastSync > 3600000) {
    syncTimeNTP();
    lastSync = millis();
  }

  delay(1000);
}

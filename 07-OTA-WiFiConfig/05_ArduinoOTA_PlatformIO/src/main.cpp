#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

// ArduinoOTA: nap code qua WiFi thang tu PlatformIO, bam Upload la xong -
// tien hon bai 03_OTA_LAN_WebBrowser (phai build ra file .bin roi mo trinh
// duyet chon file). Rat hop khi board da lap vao tu dien, tran nha... kho
// cam cap USB.
//
// Cach dung:
//  1. Lan dau nap binh thuong qua cap USB (env mac dinh trong platformio.ini).
//  2. Mo Serial Monitor, thay dong "OTA san sang" la duoc.
//  3. Tu lan sau: pio run -e ota -t upload (hoac chon env "ota" trong thanh
//     PlatformIO roi bam Upload). Code moi van phai giu phan ArduinoOTA ben
//     duoi, neu bo di thi lan sau lai phai nap bang cap.
//
// Luu y: May tinh va ESP32 phai cung mang WiFi. Windows Firewall co the chan
// ket noi nguoc tu ESP32 ve may tinh - neu bi treo o "Waiting for device",
// cho phep Python qua Firewall.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

const char* OTA_HOSTNAME = "esp32-ota";       // phai trung upload_port trong platformio.ini
const char* OTA_PASSWORD = "Mat_Khau_OTA";    // phai trung --auth trong platformio.ini

const int LED_PIN = 2;

void setupOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);   // khong dat mat khau thi ai cung mang cung nap de duoc code

  ArduinoOTA.onStart([]() {
    // Neu dang ghi file SPIFFS/LittleFS thi nen dung lai o day truoc khi nap
    Serial.println("\nBat dau nap firmware qua OTA...");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Tien do: %u%%\r", progress * 100 / total);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nNap xong, ESP32 se tu khoi dong lai");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Loi OTA [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("sai mat khau");
    else if (error == OTA_BEGIN_ERROR) Serial.println("khong du cho trong flash");
    else if (error == OTA_CONNECT_ERROR) Serial.println("khong ket noi duoc may tinh");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("loi nhan du lieu");
    else if (error == OTA_END_ERROR) Serial.println("loi ket thuc");
  });

  ArduinoOTA.begin();   // tu khoi dong ca mDNS voi ten OTA_HOSTNAME
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  setupOTA();
  Serial.printf("OTA san sang: %s.local (%s)\n", OTA_HOSTNAME, WiFi.localIP().toString().c_str());
}

void loop() {
  // Phai goi lien tuc de ESP32 nghe yeu cau nap tu PlatformIO.
  // Tranh delay() dai trong loop(), neu khong PlatformIO se bao het thoi gian cho.
  ArduinoOTA.handle();

  // Nhay LED bang millis() - sua chu ky nhay roi nap lai qua OTA de thu
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 500) {
    lastBlink = millis();
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}

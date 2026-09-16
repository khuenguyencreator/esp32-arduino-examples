#include <Arduino.h>

#include <WiFi.h>
#include <WiFiUdp.h>

// Bai nay minh hoa giao thuc UDP - khac TCP o cho KHONG can "bat tay" thiet
// lap ket noi truoc, chi viec gui goi tin di la xong, nhanh hon nhung khong
// dam bao goi tin toi noi (hay dung cho video/audio streaming, game, NTP...).
// Dung lenh sau tren may tinh (co netcat) de gui thu 1 goi UDP:
//   echo "hello esp32" | nc -u -w1 <IP_ESP32> 4210

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

const uint16_t udpPort = 4210;
WiFiUDP udp;

char incomingPacket[255];

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

  udp.begin(udpPort);
  Serial.print("Dang lang nghe UDP tai cong: ");
  Serial.println(udpPort);
}

void loop() {
  int packetSize = udp.parsePacket();

  if (packetSize) {
    // biet duoc goi tin nay gui tu dau (IP + cong) de con tra loi lai dung noi
    IPAddress remoteIp = udp.remoteIP();
    int remotePort = udp.remotePort();

    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0'; // ket thuc chuoi C bang ky tu null
    }

    Serial.print("Nhan goi UDP tu ");
    Serial.print(remoteIp);
    Serial.print(":");
    Serial.print(remotePort);
    Serial.print(" - noi dung: ");
    Serial.println(incomingPacket);

    // gui phan hoi nguoc lai dung IP va cong vua nhan duoc
    udp.beginPacket(remoteIp, remotePort);
    udp.print("ESP32 da nhan: ");
    udp.print(incomingPacket);
    udp.endPacket();
  }
}

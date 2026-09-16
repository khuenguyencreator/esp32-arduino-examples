#include <Arduino.h>

#include <WiFi.h>

// Bai nay minh hoa TCP/IP o mot lop THAP hon HTTP - khong co header, khong
// co cu phap GET/POST, chi la 1 ket noi TCP tho nhan/gui du lieu dang text
// tuy y. Dung lenh "telnet <IP_ESP32> 8888" hoac "nc <IP_ESP32> 8888" tren
// may tinh de ket noi thu.

const int led = 26;
const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

const uint16_t tcpPort = 8888; // cong tuy chon, khac cong 80 mac dinh cua HTTP
WiFiServer tcpServer(tcpPort);

void setup() {
  Serial.begin(115200);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

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
  Serial.print("TCP server dang lang nghe tai cong: ");
  Serial.println(tcpPort);

  tcpServer.begin();
}

void loop() {
  WiFiClient client = tcpServer.available();

  if (client) {
    Serial.println("Co client TCP moi ket noi toi");
    client.println("Xin chao tu ESP32! Go LED_ON hoac LED_OFF roi Enter:");

    while (client.connected()) {
      if (client.available()) {
        // doc 1 dong text client gui toi (ket thuc boi ky tu xuong dong)
        String line = client.readStringUntil('\n');
        line.trim(); // bo khoang trang/ky tu \r thua o cuoi dong
        Serial.print("Nhan tu client: ");
        Serial.println(line);

        if (line == "LED_ON") {
          digitalWrite(led, HIGH);
          client.println("Da bat LED");
        } else if (line == "LED_OFF") {
          digitalWrite(led, LOW);
          client.println("Da tat LED");
        } else if (line.length() > 0) {
          // khong khop lenh nao, echo lai nguyen van cho client (kieu TCP echo)
          client.println("Echo: " + line);
        }
      }
    }

    client.stop();
    Serial.println("Client TCP ngat ket noi");
  }
}

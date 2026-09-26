#include <Arduino.h>

#include <WiFi.h>
#include <esp_now.h>

// Board Receiver - nhan du lieu qua ESP-NOW tu board Sender, dieu khien Led
// theo trang thai nut bam cua Sender gui toi.

const int led = 26;

// Cau truc du lieu phai giong HET voi ben Sender
typedef struct struct_message {
  bool buttonPressed;
} struct_message;

struct_message receivedData;

void onDataRecv(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  Serial.print("Nhan du lieu tu MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", macAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" - buttonPressed = ");
  Serial.println(receivedData.buttonPressed ? "true" : "false");

  digitalWrite(led, receivedData.buttonPressed ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  WiFi.mode(WIFI_STA); // ESP-NOW can WiFi o che do STA, khong can ket noi router nao

  Serial.print("Dia chi MAC cua board nay (Receiver) - copy vao code Sender: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Loi khoi tao ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // Khong can lam gi trong loop - moi xu ly xay ra trong callback onDataRecv
}

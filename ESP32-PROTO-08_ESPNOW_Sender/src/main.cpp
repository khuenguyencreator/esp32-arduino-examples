#include <Arduino.h>

#include <WiFi.h>
#include <esp_now.h>

// Bai nay minh hoa ESP-NOW - giao thuc giao tiep truc tiep giua 2 ESP32,
// KHONG can router/WiFi network nao ca, khac han cac bai truoc phai co
// cung 1 mang WiFi moi noi chuyen duoc voi nhau.

const int button = 4;

// Thay bang dia chi MAC that cua board Receiver - lay tu Serial Monitor
// cua board do sau khi nap code ESP32-PROTO-08_ESPNOW_Receiver
uint8_t receiverMac[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

typedef struct struct_message {
  bool buttonPressed;
} struct_message;

struct_message myData;

int lastButtonState = HIGH;

void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("Gui du lieu: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thanh cong" : "That bai");
}

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Loi khoi tao ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Loi them peer");
    return;
  }

  Serial.print("Dia chi MAC cua board nay (Sender): ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  int buttonState = digitalRead(button);

  if (buttonState != lastButtonState) {
    myData.buttonPressed = (buttonState == LOW);

    esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));

    if (result == ESP_OK) {
      Serial.println("Da gui trang thai nut bam qua ESP-NOW");
    } else {
      Serial.println("Loi khi gui du lieu");
    }

    lastButtonState = buttonState;
    delay(200);
  }
}

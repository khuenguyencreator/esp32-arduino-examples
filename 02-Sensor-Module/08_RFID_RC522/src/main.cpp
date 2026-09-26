#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

// Module RFID RC522 doc the tu 13.56MHz (the Mifare, the chung cu, the gui
// xe...) qua giao tiep SPI. Bai nay lam khoa cua don gian: quet the, neu UID
// nam trong danh sach cho phep thi bat LED xanh (hoac kich relay mo cua),
// nguoc lai bat LED do.
//
// Noi day (SPI mac dinh - VSPI):
//   SDA(SS) -> GPIO5   SCK  -> GPIO18   MOSI -> GPIO23
//   MISO    -> GPIO19  RST  -> GPIO27   3.3V -> 3.3V (KHONG cap 5V)

#define SS_PIN     5
#define RST_PIN    27
#define LED_OK     26   // LED xanh / relay mo cua
#define LED_DENIED 25   // LED do

MFRC522 rfid(SS_PIN, RST_PIN);

// Danh sach UID duoc phep - quet the lan dau, copy UID in ra Serial vao day
const byte ALLOWED_UIDS[][4] = {
  {0xDE, 0xAD, 0xBE, 0xEF},
  {0x12, 0x34, 0x56, 0x78},
};
const int ALLOWED_COUNT = sizeof(ALLOWED_UIDS) / sizeof(ALLOWED_UIDS[0]);

bool isAllowed(const MFRC522::Uid &uid) {
  if (uid.size != 4) return false;   // bai nay chi xu ly the UID 4 byte (loai pho bien nhat)
  for (int i = 0; i < ALLOWED_COUNT; i++) {
    if (memcmp(uid.uidByte, ALLOWED_UIDS[i], 4) == 0) return true;
  }
  return false;
}

void printUid(const MFRC522::Uid &uid) {
  Serial.print("UID: {");
  for (byte i = 0; i < uid.size; i++) {
    Serial.printf("0x%02X", uid.uidByte[i]);
    if (i < uid.size - 1) Serial.print(", ");
  }
  Serial.println("}");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_DENIED, OUTPUT);

  SPI.begin();
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();   // in ra phien ban chip, thay 0x00/0xFF la dang noi sai day

  Serial.println("Dua the lai gan dau doc...");
}

void loop() {
  // Chua co the moi hoac doc loi thi bo qua vong nay
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  printUid(rfid.uid);

  if (isAllowed(rfid.uid)) {
    Serial.println("-> The hop le, mo cua");
    digitalWrite(LED_OK, HIGH);
    delay(2000);
    digitalWrite(LED_OK, LOW);
  } else {
    Serial.println("-> The khong co trong danh sach");
    digitalWrite(LED_DENIED, HIGH);
    delay(1000);
    digitalWrite(LED_DENIED, LOW);
  }

  // Dung giao tiep voi the hien tai, neu khong lan quet sau se khong nhan the moi
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

#include <Arduino.h>

// Bai nay doc 2 cam bien co san ben trong chip ESP32 (ban goc, khong phai
// S2/S3/C3), khong can noi them linh kien nao:
//  - Cam bien Hall: do tu truong, dua nam cham lai gan mat chip (phan vo kim
//    loai) la gia tri thay doi ro rang.
//  - Cam bien nhiet do noi: do nhiet do cua chinh con chip (khong phai nhiet
//    do phong).
//
// Luu y: ham hallRead() chi co tren Arduino core 2.x - tu core 3.x Espressif
// da bo cam bien Hall khoi API. PlatformIO "platform = espressif32" hien van
// dung core 2.x nen bai nay chay binh thuong.

const int SAMPLE_COUNT = 50; // lay trung binh nhieu lan vi cam bien Hall rat nhieu

int readHallAverage() {
  long sum = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    sum += hallRead();
  }
  return sum / SAMPLE_COUNT;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Doc cam bien Hall va nhiet do noi cua ESP32");

  // Cam bien Hall dung chung chan GPIO36 va GPIO39 ben trong chip,
  // khong noi gi vao 2 chan nay khi chay bai nay.
}

void loop() {
  int hall = readHallAverage();
  float chipTemp = temperatureRead();

  Serial.print("Hall: ");
  Serial.print(hall);
  Serial.print("\tNhiet do chip: ");
  Serial.print(chipTemp, 1);
  Serial.println(" C");

  // Nhieu chip ESP32 doi moi khong con cam bien nhiet do (bi bo khoi silicon),
  // ham se luon tra ve 53.3 C. Neu gia tri dung yen o muc nay thi chip cua
  // ban thuoc loai do, khong phai loi code.
  if (chipTemp > 53.2 && chipTemp < 53.4) {
    Serial.println("  -> Chip nay co the khong ho tro cam bien nhiet do noi");
  }

  delay(500);
}

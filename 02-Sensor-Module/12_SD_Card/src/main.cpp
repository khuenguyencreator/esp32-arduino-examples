#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

// Doc/ghi the microSD qua module doc the SPI (loai co san IC chuyen muc
// 3.3V/5V). Khac bai ESP32-CAM dung SD_MMC (giao tiep SDIO), board ESP32
// DevKit thuong khong co khe the nen dung SPI la cach pho bien nhat.
//
// Noi day (SPI mac dinh - VSPI):
//   CS -> GPIO5   SCK -> GPIO18   MOSI -> GPIO23   MISO -> GPIO19
//   VCC -> 5V (module co IC ha ap) hoac 3.3V (module khong co IC)   GND -> GND
//
// The nho can format FAT32. The tren 32GB thuong la exFAT, phai format lai.

const int SD_CS = 5;

void listDir(fs::FS &fs, const char* dirname) {
  Serial.printf("Danh sach file trong %s:\n", dirname);
  File root = fs.open(dirname);
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.printf("  [DIR] %s\n", file.name());
    } else {
      Serial.printf("  %s (%u byte)\n", file.name(), file.size());
    }
    file = root.openNextFile();
  }
}

void writeFile(fs::FS &fs, const char* path, const char* message) {
  File file = fs.open(path, FILE_WRITE);   // FILE_WRITE ghi de noi dung cu
  if (!file) {
    Serial.println("Khong mo duoc file de ghi");
    return;
  }
  file.print(message);
  file.close();
  Serial.printf("Da ghi file %s\n", path);
}

void appendFile(fs::FS &fs, const char* path, const char* message) {
  File file = fs.open(path, FILE_APPEND);  // FILE_APPEND ghi noi vao cuoi
  if (!file) {
    Serial.println("Khong mo duoc file de ghi noi");
    return;
  }
  file.print(message);
  file.close();
  Serial.printf("Da ghi noi vao %s\n", path);
}

void readFile(fs::FS &fs, const char* path) {
  File file = fs.open(path);
  if (!file) {
    Serial.println("Khong mo duoc file de doc");
    return;
  }
  Serial.printf("--- Noi dung %s ---\n", path);
  while (file.available()) {
    Serial.write(file.read());
  }
  Serial.println("\n------------------");
  file.close();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  if (!SD.begin(SD_CS)) {
    Serial.println("Khong khoi tao duoc the SD - kiem tra day noi va dinh dang FAT32");
    return;
  }

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Chua cam the nho");
    return;
  }

  const char* typeName = cardType == CARD_MMC ? "MMC" : cardType == CARD_SD ? "SDSC" :
                         cardType == CARD_SDHC ? "SDHC" : "Khong ro";
  Serial.printf("Loai the: %s, dung luong: %llu MB\n", typeName, SD.cardSize() / (1024 * 1024));

  writeFile(SD, "/hello.txt", "Xin chao tu ESP32!\n");
  appendFile(SD, "/hello.txt", "Dong nay duoc ghi noi them.\n");
  readFile(SD, "/hello.txt");

  listDir(SD, "/");

  Serial.printf("Da dung %llu / %llu MB\n", SD.usedBytes() / (1024 * 1024), SD.totalBytes() / (1024 * 1024));
}

void loop() {
}

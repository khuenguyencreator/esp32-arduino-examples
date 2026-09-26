#include <Arduino.h>

#include <LittleFS.h>

// LittleFS la he thong file thay the cho SPIFFS (SPIFFS da ngung duoc phat
// trien). Uu diem chinh:
//  - Co thu muc that (SPIFFS chi gia lap thu muc bang ten file co dau /)
//  - Chiu mat dien giua chung tot hon, it bi hong file
//  - Doc/ghi nhanh hon khi bo nho da day
// Cach dung gan nhu y het SPIFFS, chi doi SPIFFS.xxx thanh LittleFS.xxx.
//
// Bai nay: dem so lan khoi dong (luu trong file), ghi log moi lan khoi dong
// vao thu muc /logs, roi liet ke toan bo file va dung luong con trong.

const char* BOOT_COUNT_FILE = "/boot_count.txt";
const char* LOG_DIR = "/logs";
const char* LOG_FILE = "/logs/boot.log";

int readBootCount() {
  if (!LittleFS.exists(BOOT_COUNT_FILE)) return 0;
  File file = LittleFS.open(BOOT_COUNT_FILE, "r");
  int value = file.parseInt();
  file.close();
  return value;
}

void writeBootCount(int value) {
  File file = LittleFS.open(BOOT_COUNT_FILE, "w");
  file.print(value);
  file.close();
}

void appendLog(int bootCount) {
  if (!LittleFS.exists(LOG_DIR)) {
    LittleFS.mkdir(LOG_DIR);   // LittleFS ho tro tao thu muc that
  }
  File file = LittleFS.open(LOG_FILE, "a");   // "a" = ghi noi vao cuoi file
  file.printf("Lan khoi dong thu %d\n", bootCount);
  file.close();
}

void listDir(const char* path, int depth) {
  File dir = LittleFS.open(path);
  File entry = dir.openNextFile();
  while (entry) {
    for (int i = 0; i < depth; i++) Serial.print("  ");
    if (entry.isDirectory()) {
      Serial.printf("[DIR] %s\n", entry.name());
      String subPath = String(path) + (String(path).endsWith("/") ? "" : "/") + entry.name();
      listDir(subPath.c_str(), depth + 1);
    } else {
      Serial.printf("%s (%u byte)\n", entry.name(), entry.size());
    }
    entry = dir.openNextFile();
  }
}

void printFile(const char* path) {
  File file = LittleFS.open(path, "r");
  Serial.printf("--- Noi dung %s ---\n", path);
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // true = tu format neu partition chua duoc format (lan chay dau tien)
  if (!LittleFS.begin(true)) {
    Serial.println("Loi: khong mount duoc LittleFS");
    return;
  }

  int bootCount = readBootCount() + 1;
  writeBootCount(bootCount);
  appendLog(bootCount);

  Serial.printf("ESP32 da khoi dong %d lan (nhan nut EN de tang so dem)\n\n", bootCount);

  Serial.println("--- Danh sach file ---");
  listDir("/", 0);
  Serial.println();

  printFile(LOG_FILE);

  Serial.printf("\nDung luong: da dung %u / %u byte\n", LittleFS.usedBytes(), LittleFS.totalBytes());
}

void loop() {
}

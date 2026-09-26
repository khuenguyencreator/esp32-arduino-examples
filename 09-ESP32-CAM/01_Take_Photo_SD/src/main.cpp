#include <Arduino.h>

#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

// Bai nay dung board ESP32-CAM (AI-Thinker) - khac han board ESP32 DoIT
// devkit dung xuyen suot serie, vi board nay co san module camera OV2640
// va khe cam the nho microSD ngay tren mach.

// Khai bao chan cam bien camera theo dung sơ đồ board AI-Thinker
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const int flashLedPin = 4;   // den flash tren board, dung tam sang khi chup
const int buttonPin = 13;    // nut nhan chup anh, dau kia noi GND

int photoCount = 0;

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  return esp_camera_init(&config) == ESP_OK;
}

void takePhoto() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Chup anh that bai");
    return;
  }

  String path = "/photo_" + String(photoCount) + ".jpg";
  File file = SD_MMC.open(path, FILE_WRITE);

  if (!file) {
    Serial.println("Khong mo duoc file tren the nho");
  } else {
    file.write(fb->buf, fb->len);
    Serial.print("Da luu anh: ");
    Serial.println(path);
    photoCount++;
  }

  file.close();
  esp_camera_fb_return(fb);
}

void setup() {
  Serial.begin(115200);
  pinMode(flashLedPin, OUTPUT);
  digitalWrite(flashLedPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);

  if (!initCamera()) {
    Serial.println("Khoi tao camera that bai");
    return;
  }

  // true = che do 1-bit, bat buoc tren AI-Thinker vi cac chan 4-bit
  // trung voi chan camera dang dung
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("Khoi tao the nho that bai - kiem tra da cam the chua");
    return;
  }

  // Bo qua cac file anh da co tren the tu lan chay truoc, tranh ghi de
  while (SD_MMC.exists("/photo_" + String(photoCount) + ".jpg")) {
    photoCount++;
  }

  Serial.println("San sang - nhan nut de chup anh");
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(flashLedPin, HIGH);
    delay(100);
    takePhoto();
    digitalWrite(flashLedPin, LOW);

    delay(1000); // chong doi nhieu anh lien tuc khi giu nut
  }
}

#include <Arduino.h>

#include <TFT_eSPI.h>

// Man hinh TFT mau (ILI9341 240x320 hoac ST7789) giao tiep SPI, dung thu vien
// TFT_eSPI - thu vien nhanh nhat cho ESP32 hien nay. Cau hinh chan va loai
// driver nam het trong build_flags cua platformio.ini, khong can sua file
// User_Setup.h trong thu vien.
//
// Bai nay ve giao dien do dien ap bien tro: so to kieu LED 7 doan va thanh
// ngang the hien muc gia tri.
//
// Noi day:
//   SCK -> GPIO18   MOSI(SDA) -> GPIO23   MISO -> GPIO19 (co the bo)
//   CS  -> GPIO15   DC(RS)    -> GPIO2    RST  -> GPIO4
//   LED -> 3.3V     VCC -> 3.3V           GND  -> GND
//   Bien tro: chan giua -> GPIO34

TFT_eSPI tft = TFT_eSPI();

const int POT_PIN = 34;
const int BAR_X = 20, BAR_Y = 170, BAR_W = 280, BAR_H = 30;

void drawStaticUI() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);                      // can giua theo chieu ngang
  tft.drawString("ESP32 Voltmeter", tft.width() / 2, 10, 4);   // font 4 (26px)

  tft.drawRect(BAR_X - 2, BAR_Y - 2, BAR_W + 4, BAR_H + 4, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.drawString("0V", BAR_X, BAR_Y + BAR_H + 8, 2);
  tft.setTextDatum(TR_DATUM);
  tft.drawString("3.3V", BAR_X + BAR_W, BAR_Y + BAR_H + 8, 2);
}

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1);   // xoay ngang: 320 x 240
  drawStaticUI();
}

void loop() {
  int raw = analogRead(POT_PIN);
  float voltage = raw * 3.3 / 4095.0;

  // Chi ve lai phan thay doi, khong fillScreen moi vong -> man hinh khong nhap nhay
  char text[8];
  snprintf(text, sizeof(text), "%.2f", voltage);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);          // co mau nen -> ve de len so cu
  tft.setTextDatum(MC_DATUM);
  tft.setTextPadding(tft.textWidth("8.88", 7));    // xoa du cho khi so co it chu so hon
  tft.drawString(text, tft.width() / 2, 100, 7);   // font 7 = kieu LED 7 doan

  int fill = map(raw, 0, 4095, 0, BAR_W);
  uint16_t color = voltage < 1.1 ? TFT_GREEN : voltage < 2.2 ? TFT_YELLOW : TFT_RED;
  tft.fillRect(BAR_X, BAR_Y, fill, BAR_H, color);
  tft.fillRect(BAR_X + fill, BAR_Y, BAR_W - fill, BAR_H, TFT_DARKGREY);

  delay(100);
}

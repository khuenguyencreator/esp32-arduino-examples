#include <Arduino.h>

#include "driver/rmt.h"

// Bai nay dieu khien dai LED WS2812 bang ngoai vi RMT (Remote Control) cua
// ESP32, khong dung thu vien NeoPixel. RMT duoc thiet ke de phat/thu chuoi
// xung co do rong chinh xac toi vai chuc nano giay - dung thu ma WS2812 can.
// Sau khi nap du lieu, RMT tu phat xung bang phan cung, CPU khong phai dem
// thoi gian bang tay nen khong bi ngat (interrupt) lam sai mau.
//
// Bai du an 10-Projects/02_Neopixel_Light dung thu vien Adafruit_NeoPixel -
// ben duoi thu vien do tren ESP32 cung chinh la RMT nhu bai nay.

#define LED_PIN     GPIO_NUM_26
#define LED_COUNT   8
#define RMT_CH      RMT_CHANNEL_0

// clk_div = 2 -> RMT dem o 80MHz / 2 = 40MHz, moi tick = 25ns
// Thoi gian theo datasheet WS2812B (sai so cho phep +-150ns):
#define T0H 16  // bit 0: muc cao 0.4us
#define T0L 34  //        muc thap 0.85us
#define T1H 32  // bit 1: muc cao 0.8us
#define T1L 18  //        muc thap 0.45us

uint8_t pixels[LED_COUNT * 3];               // thu tu byte cua WS2812 la G-R-B
rmt_item32_t items[LED_COUNT * 24];          // moi LED 24 bit, moi bit 1 item

void setPixel(int index, uint8_t r, uint8_t g, uint8_t b) {
  pixels[index * 3 + 0] = g;
  pixels[index * 3 + 1] = r;
  pixels[index * 3 + 2] = b;
}

void show() {
  int n = 0;
  for (int i = 0; i < LED_COUNT * 3; i++) {
    for (int bit = 7; bit >= 0; bit--) {   // WS2812 nhan bit cao (MSB) truoc
      bool one = pixels[i] & (1 << bit);
      items[n].level0 = 1;
      items[n].duration0 = one ? T1H : T0H;
      items[n].level1 = 0;
      items[n].duration1 = one ? T1L : T0L;
      n++;
    }
  }
  // true = cho phat xong moi tra ve. Sau do chan giu muc thap > 50us
  // (thoi gian reset), dai LED se chot mau vua nhan.
  rmt_write_items(RMT_CH, items, n, true);
}

// Doi mau HSV sang RGB, hue 0-255, de lam hieu ung cau vong
void hsvToRgb(uint8_t hue, uint8_t &r, uint8_t &g, uint8_t &b) {
  uint8_t region = hue / 43;
  uint8_t remainder = (hue - region * 43) * 6;
  uint8_t q = 255 - remainder;
  uint8_t t = remainder;
  switch (region) {
    case 0:  r = 255; g = t;   b = 0;   break;
    case 1:  r = q;   g = 255; b = 0;   break;
    case 2:  r = 0;   g = 255; b = t;   break;
    case 3:  r = 0;   g = q;   b = 255; break;
    case 4:  r = t;   g = 0;   b = 255; break;
    default: r = 255; g = 0;   b = q;   break;
  }
}

void setup() {
  Serial.begin(115200);

  rmt_config_t config = RMT_DEFAULT_CONFIG_TX(LED_PIN, RMT_CH);
  config.clk_div = 2;
  rmt_config(&config);
  rmt_driver_install(config.channel, 0, 0);

  Serial.println("RMT da san sang, bat dau hieu ung cau vong");
}

void loop() {
  static uint8_t offset = 0;

  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t r, g, b;
    hsvToRgb(offset + i * (256 / LED_COUNT), r, g, b);
    // Chia 8 de giam do sang, tranh choi mat va tranh sut nguon USB
    setPixel(i, r / 8, g / 8, b / 8);
  }
  show();

  offset++;
  delay(20);
}

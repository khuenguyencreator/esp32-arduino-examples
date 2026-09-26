#include <Arduino.h>

#include <Wire.h>
#include <RTClib.h>

// DS3231 la module dong ho thoi gian thuc (RTC) co pin CR2032 nuoi rieng,
// giu gio chinh xac ca khi ESP32 mat dien - khac NTP (bai WiFiManager_NTP)
// phai co internet moi lay duoc gio. DS3231 co bu nhiet nen sai so chi
// khoang 1-2 phut/nam, tot hon han DS1307.
//
// Noi day I2C: SDA -> GPIO21, SCL -> GPIO22, VCC -> 3.3V, GND -> GND.
//
// Chinh gio qua Serial Monitor: go dung dinh dang
//   2026-09-26 10:30:00
// roi Enter.

RTC_DS3231 rtc;

const char* WEEKDAYS[7] = {"CN", "T2", "T3", "T4", "T5", "T6", "T7"};

void setTimeFromSerial() {
  String line = Serial.readStringUntil('\n');
  int y, mo, d, h, mi, s;
  if (sscanf(line.c_str(), "%d-%d-%d %d:%d:%d", &y, &mo, &d, &h, &mi, &s) == 6) {
    rtc.adjust(DateTime(y, mo, d, h, mi, s));
    Serial.println("Da chinh gio cho DS3231");
  } else {
    Serial.println("Sai dinh dang, go theo mau: 2026-09-26 10:30:00");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Khong tim thay DS3231 - kiem tra lai day I2C");
    while (true) delay(1000);
  }

  // lostPower() = true khi pin CR2032 het hoac moi lap pin lan dau,
  // luc do lay tam gio luc bien dich code lam gio ban dau.
  if (rtc.lostPower()) {
    Serial.println("DS3231 bi mat nguon, dat lai gio theo thoi diem bien dich");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  if (Serial.available()) {
    setTimeFromSerial();
  }

  DateTime now = rtc.now();
  Serial.printf("%s %02d/%02d/%04d %02d:%02d:%02d",
                WEEKDAYS[now.dayOfTheWeek()], now.day(), now.month(), now.year(),
                now.hour(), now.minute(), now.second());

  // DS3231 co san cam bien nhiet do de tu bu sai so thach anh, doc ra duoc luon
  Serial.printf("  |  Nhiet do module: %.2f C\n", rtc.getTemperature());

  delay(1000);
}

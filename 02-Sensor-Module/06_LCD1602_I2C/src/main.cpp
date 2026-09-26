#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD 1602 (16 cot x 2 dong) co gan san module chuyen doi I2C PCF8574 phia
// sau, nen chi can 4 day: VCC, GND, SDA (GPIO21), SCL (GPIO22) thay vi 6-10
// day song song nhu LCD tron.
//
// Dia chi I2C thuong la 0x27 hoac 0x3F tuy hang san xuat module PCF8574.
// Khong chac thi nap bai 01_I2C_Scan truoc de do dia chi.

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ky tu tu ve: moi ky tu la o 5x8 diem, moi byte la 1 hang (5 bit thap)
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, heart);   // luu ky tu tu ve vao o nho so 0 cua LCD

  lcd.setCursor(0, 0);        // cot 0, dong 0
  lcd.print("Hello ESP32 ");
  lcd.write(0);               // in ky tu tu ve so 0
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Uptime: ");
  lcd.print(millis() / 1000);
  lcd.print(" s   ");         // them dau cach de xoa ky tu thua cua so cu dai hon

  delay(1000);
}

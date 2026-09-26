#include <Arduino.h>

#include <Keypad.h>

// Ban phim ma tran 4x4 chi can 8 chan thay vi 16: thu vien Keypad lan luot
// keo tung cot xuong muc thap roi doc 4 hang de biet phim nao dang duoc bam
// (quet ma tran - giong y tuong quet LED 7 doan).
//
// Bai nay lam o khoa mat khau: nhap so, bam # de xac nhan, * de xoa.
//
// Luu y chon chan: tranh GPIO0, 2, 12, 15 (chan strapping) - thu vien keo len
// cac chan hang, neu GPIO12 bi keo len luc khoi dong ESP32 se khong boot duoc.

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {13, 14, 27, 26};   // noi tu trai sang phai theo 4 chan dau cua keypad
byte colPins[COLS] = {25, 33, 32, 4};    // 4 chan con lai

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String PASSWORD = "1234";
const int LED_PIN = 2;   // LED co san tren board DOIT

String input = "";

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Nhap mat khau roi bam #, bam * de xoa");
}

void loop() {
  char key = keypad.getKey();   // tra ve 0 neu khong co phim nao vua bam
  if (!key) return;

  if (key == '*') {
    input = "";
    Serial.println("\nDa xoa");
  } else if (key == '#') {
    if (input == PASSWORD) {
      Serial.println("\nMat khau dung - mo khoa");
      digitalWrite(LED_PIN, HIGH);
      delay(2000);
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.println("\nSai mat khau");
    }
    input = "";
  } else {
    input += key;
    Serial.print('*');   // chi in dau * de khong lo mat khau tren man hinh
  }
}

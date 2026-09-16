#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Du an "may choi dien tu" mini: game do phan xa - LED bat sang sau 1
// khoang cho ngau nhien, nguoi choi bam nut cang nhanh cang tot, man hinh
// OLED hien thoi gian phan ung (mili-giay) va ky luc tot nhat tu truoc toi
// gio. Ket hop lai kien thuc GPIO, ngat, va OLED tu cac bai truoc.

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int ledPin = 26;
const int buttonPin = 27;

unsigned long bestTime = 0;

void showMessage(String line1, String line2 = "") {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println(line1);
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.println(line2);
  display.display();
}

void playRound() {
  showMessage("Cho...", "Nhan nut khi den sang");
  digitalWrite(ledPin, LOW);

  unsigned long waitTime = random(1500, 4000); // cho ngau nhien 1.5-4 giay
  unsigned long waitStart = millis();

  while (millis() - waitStart < waitTime) {
    if (digitalRead(buttonPin) == LOW) {
      showMessage("Bam som qua!", "Thu lai nhe");
      delay(2000);
      return;
    }
  }

  digitalWrite(ledPin, HIGH);
  unsigned long litAt = millis();

  while (digitalRead(buttonPin) == HIGH) {
    // cho nguoi choi bam nut
  }

  unsigned long reactionTime = millis() - litAt;
  digitalWrite(ledPin, LOW);

  if (bestTime == 0 || reactionTime < bestTime) {
    bestTime = reactionTime;
    showMessage(String(reactionTime) + " ms", "Ky luc moi!");
  } else {
    showMessage(String(reactionTime) + " ms", "Tot nhat: " + String(bestTime) + " ms");
  }

  Serial.print("Thoi gian phan xa: ");
  Serial.print(reactionTime);
  Serial.println(" ms");

  delay(3000);
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Khong tim thay man hinh OLED");
    while (true) delay(1000);
  }
  display.setTextColor(SSD1306_WHITE);

  showMessage("Game phan xa", "Nhan nut de bat dau");
  while (digitalRead(buttonPin) == HIGH) {
    delay(10);
  }
}

void loop() {
  playRound();
}

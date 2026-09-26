#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

// Den nhieu mau dung dai LED dia chi hoa duoc (WS2811/WS2812), dieu khien
// tung LED rieng le chi voi 1 chan tin hieu duy nhat - khac han LED don
// (RGB LED thuong) can toi 3 chan PWM. Bai nay lam 3 hieu ung: mau don,
// chay duoi (chase), va cau vong (rainbow), chuyen doi bang 1 nut nhan.

#define LED_PIN 26
#define LED_COUNT 12 // so luong LED tren dai, sua theo dai LED thuc te cua ban

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int buttonPin = 27;
int currentEffect = 0;
int lastButtonState = HIGH;

void effectSolidColor() {
  strip.fill(strip.Color(0, 150, 255)); // mau xanh duong co dinh
  strip.show();
}

void effectChase() {
  static int position = 0;
  strip.clear();
  strip.setPixelColor(position, strip.Color(255, 0, 0));
  strip.show();
  position = (position + 1) % LED_COUNT;
  delay(100);
}

uint32_t wheel(byte pos) {
  pos = 255 - pos;
  if (pos < 85) {
    return strip.Color(255 - pos * 3, 0, pos * 3);
  } else if (pos < 170) {
    pos -= 85;
    return strip.Color(0, pos * 3, 255 - pos * 3);
  } else {
    pos -= 170;
    return strip.Color(pos * 3, 255 - pos * 3, 0);
  }
}

void effectRainbow() {
  static byte hue = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, wheel((i * 256 / LED_COUNT + hue) & 255));
  }
  strip.show();
  hue++;
  delay(20);
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  strip.begin();
  strip.setBrightness(80); // gioi han do sang (0-255) tranh dai LED tieu thu qua nhieu dong
  strip.show();

  Serial.println("San sang - nhan nut de doi hieu ung");
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    currentEffect = (currentEffect + 1) % 3;
    Serial.print("Doi sang hieu ung: ");
    Serial.println(currentEffect);
    delay(50); // chong doi nut
  }
  lastButtonState = buttonState;

  switch (currentEffect) {
    case 0:
      effectSolidColor();
      break;
    case 1:
      effectChase();
      break;
    case 2:
      effectRainbow();
      break;
  }
}

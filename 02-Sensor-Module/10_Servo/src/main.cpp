#include <Arduino.h>

// Dieu khien servo (SG90, MG996R...) bang chinh ngoai vi LEDC (PWM) da hoc o
// bai 01-Basic/03_PWM, khong can thu vien Servo.
//
// Servo nhan xung chu ky 20ms (50Hz). Do rong xung quyet dinh goc quay:
//   ~0.5ms -> 0 do,  ~1.5ms -> 90 do,  ~2.4ms -> 180 do
// (con so chinh xac lech nhau tuy hang servo, chinh MIN_US/MAX_US cho khop)
//
// Bai nay dung bien tro (chan giua noi GPIO34) de xoay servo theo tay van.
//
// Luu y: cap nguon 5V rieng cho servo (servo keo dong lon khi quay, cap tu
// chan 5V cua board de gay reset ESP32), nho noi chung GND voi ESP32.

const int SERVO_PIN = 18;
const int POT_PIN = 34;

const int PWM_CHANNEL = 0;
const int PWM_FREQ = 50;          // 50Hz = chu ky 20ms
const int PWM_RESOLUTION = 16;    // 16 bit -> 65536 muc, chia duoc do rong xung rat min

const int MIN_US = 500;
const int MAX_US = 2400;
const int PERIOD_US = 20000;

void writeAngle(int angle) {
  angle = constrain(angle, 0, 180);
  int pulseUs = map(angle, 0, 180, MIN_US, MAX_US);
  uint32_t duty = (uint32_t)pulseUs * 65535 / PERIOD_US;
  ledcWrite(PWM_CHANNEL, duty);
}

void setup() {
  Serial.begin(115200);

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(SERVO_PIN, PWM_CHANNEL);

  writeAngle(90);   // dua ve giua khi khoi dong
}

void loop() {
  int raw = analogRead(POT_PIN);            // 0 - 4095
  int angle = map(raw, 0, 4095, 0, 180);

  writeAngle(angle);

  Serial.print("Bien tro: ");
  Serial.print(raw);
  Serial.print("\tGoc servo: ");
  Serial.println(angle);

  delay(50);
}

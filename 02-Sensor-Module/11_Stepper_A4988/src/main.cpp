#include <Arduino.h>

// Dieu khien dong co buoc (NEMA17, 17HS4401...) qua driver A4988, khong can
// thu vien: moi xung o chan STEP la dong co quay 1 buoc, chan DIR chon chieu.
//
// Noi day:
//   STEP -> GPIO26    DIR -> GPIO27    EN -> GPIO25 (muc thap = bat driver)
//   MS1/MS2/MS3 de trong (full step, 200 buoc/vong voi dong co 1.8 do)
//   VMOT/GND -> nguon 12V rieng cho dong co (co tu 100uF sat chan VMOT)
//   VDD/GND  -> 3.3V va GND cua ESP32
//
// Luu y: chinh bien tro Vref tren A4988 truoc khi chay (Vref ~ dong dinh muc
// x 8 x 0.068 voi mach dien tro Rs = 0.068 ohm). De Vref qua cao, driver va
// dong co nong rat nhanh. KHONG rut/cam day dong co khi driver dang co dien,
// A4988 se hong ngay.

const int STEP_PIN = 26;
const int DIR_PIN = 27;
const int EN_PIN = 25;

const int STEPS_PER_REV = 200;   // 360 / 1.8 do

// Quay so buoc cho truoc, co tang/giam toc tuyen tinh de khong bi truot buoc
// khi khoi dong o toc do cao.
void moveSteps(long steps, int maxRpm) {
  digitalWrite(DIR_PIN, steps > 0 ? HIGH : LOW);
  steps = labs(steps);

  // Khoang cach giua 2 xung (us) = 60s / (rpm * so buoc moi vong)
  const unsigned long fastDelay = 60000000UL / ((unsigned long)maxRpm * STEPS_PER_REV);
  const unsigned long slowDelay = fastDelay * 4;   // khoi dong o 1/4 toc do
  const long rampSteps = min(steps / 2, 100L);     // 100 buoc de tang toc

  digitalWrite(EN_PIN, LOW);   // bat driver

  for (long i = 0; i < steps; i++) {
    unsigned long stepDelay = fastDelay;
    if (i < rampSteps) {
      stepDelay = slowDelay - (slowDelay - fastDelay) * i / rampSteps;
    } else if (i >= steps - rampSteps) {
      stepDelay = slowDelay - (slowDelay - fastDelay) * (steps - 1 - i) / rampSteps;
    }

    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(5);              // A4988 can xung STEP rong toi thieu 1us
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(stepDelay - 5);
  }

  digitalWrite(EN_PIN, HIGH);  // tat driver khi dung de dong co khong nong
                               // (bo dong nay neu can dong co giu chat vi tri)
}

void setup() {
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);
}

void loop() {
  Serial.println("Quay 1 vong theo chieu kim dong ho");
  moveSteps(STEPS_PER_REV, 120);
  delay(1000);

  Serial.println("Quay nua vong nguoc lai, cham hon");
  moveSteps(-STEPS_PER_REV / 2, 30);
  delay(1000);
}

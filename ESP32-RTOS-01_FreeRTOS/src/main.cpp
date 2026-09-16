#include <Arduino.h>

// ESP32 chay san FreeRTOS ngay ben duoi Arduino framework - ham loop() ban
// thay thuc chat cung la 1 task FreeRTOS do Arduino tu tao san. Bai nay minh
// hoa cach tu tao them 2 task chay SONG SONG that su (ESP32 co 2 nhan CPU),
// thay vi nhoi het moi viec vao 1 vong loop() duy nhat roi dung delay() lam
// nghen ca chuong trinh.

const int ledBlink = 26;
const int ledHeartbeat = 27;

void taskBlink(void *parameter) {
  pinMode(ledBlink, OUTPUT);

  for (;;) {
    digitalWrite(ledBlink, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    digitalWrite(ledBlink, LOW);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void taskHeartbeat(void *parameter) {
  pinMode(ledHeartbeat, OUTPUT);

  for (;;) {
    digitalWrite(ledHeartbeat, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(ledHeartbeat, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.print("Heartbeat tu core: ");
    Serial.println(xPortGetCoreID());
  }
}

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    taskBlink,      // ham task
    "TaskBlink",    // ten task (de debug)
    2048,           // kich thuoc stack (byte)
    NULL,           // tham so truyen vao task
    1,              // do uu tien
    NULL,           // handle cua task (khong can dung o day)
    0               // ghim vao core 0
  );

  xTaskCreatePinnedToCore(
    taskHeartbeat,
    "TaskHeartbeat",
    2048,
    NULL,
    1,
    NULL,
    1               // ghim vao core 1
  );
}

void loop() {
  // Moi viec da giao cho 2 task rieng, loop() chinh khong can lam gi nua
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

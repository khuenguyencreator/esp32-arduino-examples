#include <Arduino.h>

#include <BluetoothSerial.h>

// Bai nay minh hoa Bluetooth Classic (SPP - Serial Port Profile) tren ESP32.
// Khac WiFi, Bluetooth Classic khong can mang, chi can dien thoai/may tinh
// "pair" (ghep noi) truc tiep voi ESP32 la giao tiep duoc, y het 1 cong
// Serial khong day.

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth chua duoc bat trong cau hinh - kiem tra lai board/sdkconfig
#endif

const int led = 26;
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  SerialBT.begin("ESP32-BT-LED"); // ten thiet bi hien thi khi quet Bluetooth
  Serial.println("Bluetooth Classic da san sang, ghep noi voi ten: ESP32-BT-LED");
}

void loop() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();

    Serial.print("Nhan lenh qua Bluetooth: ");
    Serial.println(command);

    if (command == "LED_ON") {
      digitalWrite(led, HIGH);
      SerialBT.println("Da bat LED");
    } else if (command == "LED_OFF") {
      digitalWrite(led, LOW);
      SerialBT.println("Da tat LED");
    } else if (command.length() > 0) {
      SerialBT.println("Lenh khong hop le, dung LED_ON hoac LED_OFF");
    }
  }
}

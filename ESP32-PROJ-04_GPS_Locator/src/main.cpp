#include <Arduino.h>

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// Thiet bi dinh vi dung module GPS NEO-6M - module chi giao tiep qua UART
// (Serial), gui lien tuc cac cau lenh NMEA tho. Thu vien TinyGPS++ lo het
// phan parse cau lenh phuc tap, chi can doc byte tho tu Serial roi dua vao
// gps.encode() la co ngay toa do da xu ly san (vi do, kinh do, so ve tinh...).

TinyGPSPlus gps;
HardwareSerial gpsSerial(1); // dung UART1 cua ESP32, tach rieng voi Serial debug qua USB

const int rxPin = 16; // noi voi chan TX cua module GPS
const int txPin = 17; // noi voi chan RX cua module GPS

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, rxPin, txPin); // toc do chuan cua NEO-6M la 9600

  Serial.println("Dang cho tin hieu GPS... (co the mat vai phut, can o ngoai troi)");
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());

    if (gps.location.isUpdated()) {
      Serial.print("Vi do: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Kinh do: ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("So ve tinh: ");
      Serial.println(gps.satellites.value());

      Serial.print("Do cao: ");
      Serial.print(gps.altitude.meters());
      Serial.println(" m");

      Serial.print("Xem tren Google Maps: https://maps.google.com/?q=");
      Serial.print(gps.location.lat(), 6);
      Serial.print(",");
      Serial.println(gps.location.lng(), 6);

      Serial.println("---");
    }
  }

  if (millis() > 10000 && gps.charsProcessed() < 10) {
    Serial.println("Khong nhan duoc du lieu tu module GPS - kiem tra lai day noi RX/TX");
    while (true) delay(1000);
  }
}

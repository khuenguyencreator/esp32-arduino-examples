#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Board Server - tao 1 GATT Service voi 1 Characteristic dieu khien duoc,
// dien thoai (dung app nRF Connect/BLE Scanner) hoac board 05-Bluetooth/03_BLE_Client
// co the ket noi toi va ghi gia tri "1"/"0" de bat/tat LED.

// UUID mau thuong dung trong cac vi du ESP32 BLE - chi can 2 ben (server/client)
// dung chung 1 cap UUID nay la nhan dung dich vu/characteristic cua nhau
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int led = 26;

BLECharacteristic *ledCharacteristic;
bool deviceConnected = false;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *server) override {
    deviceConnected = true;
    Serial.println("Co client ket noi toi BLE Server");
  }

  void onDisconnect(BLEServer *server) override {
    deviceConnected = false;
    Serial.println("Client ngat ket noi - bat dau quang cao (advertise) lai");
    server->getAdvertising()->start();
  }
};

class LedCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) override {
    String value = characteristic->getValue().c_str(); // .c_str() de chay duoc ca core 2.x (std::string) lan 3.x (String)
    if (value.length() > 0) {
      Serial.print("Nhan gia tri ghi vao characteristic: ");
      Serial.println(value);

      if (value == "1") {
        digitalWrite(led, HIGH);
      } else if (value == "0") {
        digitalWrite(led, LOW);
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  BLEDevice::init("ESP32-BLE-LED");

  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);

  ledCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
  );
  ledCharacteristic->setCallbacks(new LedCallbacks());
  ledCharacteristic->addDescriptor(new BLE2902());
  ledCharacteristic->setValue("0");

  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("BLE Server da san sang, dang quang cao voi ten: ESP32-BLE-LED");
}

void loop() {
  // Moi xu ly nam trong callback onWrite/onConnect/onDisconnect, khong can lam gi o day
  delay(1000);
}

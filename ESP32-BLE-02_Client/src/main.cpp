#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Board Client - quet (scan) tim board Server (bai ESP32-BLE-03_Server),
// ket noi toi, roi ghi gia tri "1"/"0" vao characteristic de dieu khien LED
// tren board Server tu xa, dua vao trang thai nut nhan tren board Client.

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int buttonPin = 27; // nut nhan, dau kia noi GND, dung INPUT_PULLUP

static BLEAddress *serverAddress;
static BLERemoteCharacteristic *remoteCharacteristic;
static BLEClient *client;

bool doConnect = false;
bool connected = false;
int lastButtonState = HIGH;

class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice device) override {
    if (device.haveServiceUUID() && device.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
      Serial.println("Tim thay BLE Server phu hop, dung quet...");
      BLEDevice::getScan()->stop();
      serverAddress = new BLEAddress(device.getAddress());
      doConnect = true;
    }
  }
};

bool connectToServer() {
  Serial.print("Dang ket noi toi dia chi: ");
  Serial.println(serverAddress->toString().c_str());

  client = BLEDevice::createClient();
  if (!client->connect(*serverAddress)) {
    Serial.println("Ket noi that bai");
    return false;
  }

  BLERemoteService *remoteService = client->getService(BLEUUID(SERVICE_UUID));
  if (remoteService == nullptr) {
    Serial.println("Khong tim thay Service tren Server, ngat ket noi");
    client->disconnect();
    return false;
  }

  remoteCharacteristic = remoteService->getCharacteristic(BLEUUID(CHARACTERISTIC_UUID));
  if (remoteCharacteristic == nullptr) {
    Serial.println("Khong tim thay Characteristic tren Server, ngat ket noi");
    client->disconnect();
    return false;
  }

  Serial.println("Da ket noi thanh cong toi BLE Server");
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  BLEDevice::init("ESP32-BLE-CLIENT");

  BLEScan *scan = BLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
  scan->setActiveScan(true);
  scan->start(5, false);
}

void loop() {
  if (doConnect) {
    connected = connectToServer();
    doConnect = false;
  }

  if (connected) {
    int buttonState = digitalRead(buttonPin);

    if (buttonState != lastButtonState) {
      if (buttonState == LOW) {
        Serial.println("Nut duoc nhan - gui lenh bat LED (1)");
        remoteCharacteristic->writeValue("1");
      } else {
        Serial.println("Nha nut - gui lenh tat LED (0)");
        remoteCharacteristic->writeValue("0");
      }
      lastButtonState = buttonState;
    }

    if (!client->isConnected()) {
      Serial.println("Mat ket noi toi Server, quet tim lai...");
      connected = false;
      BLEDevice::getScan()->start(5, false);
    }
  }

  delay(50);
}

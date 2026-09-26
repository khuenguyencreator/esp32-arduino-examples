#include <Arduino.h>

#include <ModbusMaster.h>

// Modbus RTU qua RS485 la chuan giao tiep pho bien nhat trong cong nghiep:
// bien tan, dong ho dien, PLC, cam bien cong nghiep deu ho tro. RS485 truyen
// vi sai tren 2 day A/B nen chong nhieu tot, keo xa toi 1200m, 1 duong day
// noi duoc toi 32 thiet bi (moi thiet bi 1 dia chi slave).
//
// Bai nay: ESP32 lam Master, doc nhiet do - do am tu cam bien XY-MD02 (SHT20
// vo nhua, rat pho bien, gia re) qua module chuyen UART-RS485 MAX485.
// Thanh ghi Input Register (ham 0x04) cua XY-MD02:
//   0x0001 = nhiet do x 10,  0x0002 = do am x 10
// Thiet bi khac chi can doi dia chi thanh ghi theo tai lieu cua hang.
//
// Noi day MAX485:
//   RO -> GPIO16 (RX2)   DI -> GPIO17 (TX2)   DE + RE noi chung -> GPIO4
//   VCC -> 5V   GND -> GND   A/B -> A/B cua cam bien (sai thu dao A-B)
// Cam bien XY-MD02 cap nguon rieng 5-30V DC.

#define RS485_RX     16
#define RS485_TX     17
#define RS485_DE_RE  4

const uint8_t SLAVE_ID = 1;        // dia chi mac dinh cua XY-MD02
const long BAUD_RATE = 9600;       // toc do mac dinh cua XY-MD02

ModbusMaster node;

// MAX485 chi co 1 cap day nen tai 1 thoi diem chi truyen HOAC nhan:
// keo DE/RE len cao truoc khi gui, ha xuong thap ngay sau khi gui xong
void preTransmission() {
  digitalWrite(RS485_DE_RE, HIGH);
}

void postTransmission() {
  digitalWrite(RS485_DE_RE, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(RS485_DE_RE, OUTPUT);
  digitalWrite(RS485_DE_RE, LOW);   // mac dinh o che do nhan

  Serial2.begin(BAUD_RATE, SERIAL_8N1, RS485_RX, RS485_TX);

  node.begin(SLAVE_ID, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Serial.println("Modbus Master san sang");
}

void loop() {
  // Doc 2 thanh ghi lien tiep bat dau tu 0x0001 bang ham 0x04 (Read Input Registers)
  uint8_t result = node.readInputRegisters(0x0001, 2);

  if (result == node.ku8MBSuccess) {
    // Nhiet do la so co dau (co the am) -> ep kieu int16_t truoc khi chia
    float temperature = (int16_t)node.getResponseBuffer(0) / 10.0;
    float humidity = node.getResponseBuffer(1) / 10.0;
    Serial.printf("Nhiet do: %.1f C\tDo am: %.1f %%\n", temperature, humidity);
  } else {
    // 0xE2 = het thoi gian cho (khong co phan hoi): kiem tra day A/B, dia chi
    // slave, toc do baud. 0x02 = sai dia chi thanh ghi.
    Serial.printf("Loi Modbus: 0x%02X\n", result);
  }

  delay(2000);
}

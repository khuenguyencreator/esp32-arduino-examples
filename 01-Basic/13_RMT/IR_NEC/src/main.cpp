#include <Arduino.h>

#include "driver/rmt.h"

// Bai nay dung ngoai vi RMT de vua THU vua PHAT tin hieu hong ngoai (IR)
// theo chuan NEC - chuan pho bien nhat tren remote TV, quat, dieu hoa gia re.
//  - Thu: mat thu VS1838B (hoac TSOP1838) nhan tin hieu tu remote, RMT tu do
//    do rong tung xung roi dua vao ring buffer, code chi viec giai ma.
//  - Phat: LED hong ngoai, RMT tu dieu che song mang 38kHz bang phan cung.
//
// Khung NEC: header (9ms muc cao + 4.5ms muc thap), 32 bit du lieu
// (dia chi, ~dia chi, lenh, ~lenh - gui bit thap truoc), 1 xung ket thuc.
// Moi bit: 560us muc cao, roi 560us thap (bit 0) hoac 1690us thap (bit 1).

#define IR_RX_PIN   GPIO_NUM_15   // chan OUT cua mat thu VS1838B
#define IR_TX_PIN   GPIO_NUM_4    // LED hong ngoai qua dien tro 100 ohm (hoac qua transistor)
#define BUTTON_PIN  0             // nut BOOT tren board, bam de phat lenh IR
#define LED_PIN     2             // LED co san tren board DOIT

#define RMT_TX_CH   RMT_CHANNEL_0
#define RMT_RX_CH   RMT_CHANNEL_2

// Lenh se phat khi bam nut - thay bang ma cua thiet bi ban muon dieu khien
// (lay bang cach bam remote that va doc ma in ra Serial Monitor)
const uint8_t SEND_ADDRESS = 0x00;
const uint8_t SEND_COMMAND = 0x45;

RingbufHandle_t rxRingBuffer = NULL;

// So sanh do rong xung co sai so 25%
bool near(uint32_t value, uint32_t target) {
  return value > target * 3 / 4 && value < target * 5 / 4;
}

// ---------------- THU ----------------

void setupReceiver() {
  rmt_config_t config = RMT_DEFAULT_CONFIG_RX(IR_RX_PIN, RMT_RX_CH);
  config.clk_div = 80;                          // 80MHz / 80 = 1MHz -> 1 tick = 1us
  config.rx_config.filter_en = true;
  config.rx_config.filter_ticks_thresh = 100;   // bo qua gai nhieu ngan hon 100 tick APB
  config.rx_config.idle_threshold = 12000;      // im lang 12ms coi nhu het 1 khung
  rmt_config(&config);
  rmt_driver_install(config.channel, 1000, 0);
  rmt_get_ringbuf_handle(config.channel, &rxRingBuffer);
  rmt_rx_start(config.channel, true);
}

// Tra ve true neu giai ma duoc khung NEC hop le
bool decodeNEC(rmt_item32_t *items, size_t count, uint8_t &address, uint8_t &command) {
  // Mat thu VS1838B dao muc: co song IR -> chan OUT xuong thap, nen o day
  // chi so sanh do rong xung, khong phu thuoc muc logic.
  if (count < 33) return false;
  if (!near(items[0].duration0, 9000) || !near(items[0].duration1, 4500)) return false;

  uint32_t data = 0;
  for (int i = 0; i < 32; i++) {
    rmt_item32_t item = items[i + 1];
    if (!near(item.duration0, 560)) return false;
    if (near(item.duration1, 1690)) {
      data |= (1UL << i);                         // NEC gui bit thap (LSB) truoc
    } else if (!near(item.duration1, 560)) {
      return false;
    }
  }

  uint8_t addr = data & 0xFF;
  uint8_t addrInv = (data >> 8) & 0xFF;
  uint8_t cmd = (data >> 16) & 0xFF;
  uint8_t cmdInv = (data >> 24) & 0xFF;
  if ((uint8_t)(cmd ^ cmdInv) != 0xFF) return false;   // byte kiem tra sai -> nhieu
  (void)addrInv; // mot so remote dung dia chi 16 bit, khong kiem tra byte nay

  address = addr;
  command = cmd;
  return true;
}

void checkReceiver() {
  size_t length = 0;
  rmt_item32_t *items = (rmt_item32_t *)xRingbufferReceive(rxRingBuffer, &length, 0);
  if (items == NULL) return;

  size_t count = length / sizeof(rmt_item32_t);
  uint8_t address, command;

  if (decodeNEC(items, count, address, command)) {
    Serial.printf("Nhan NEC: dia chi 0x%02X, lenh 0x%02X\n", address, command);
    if (command == SEND_COMMAND) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));   // dao LED khi nhan dung lenh
    }
  } else if (count >= 2 && near(items[0].duration0, 9000) && near(items[0].duration1, 2250)) {
    Serial.println("Nhan ma lap lai (dang giu nut tren remote)");
  }

  vRingbufferReturnItem(rxRingBuffer, (void *)items);
}

// ---------------- PHAT ----------------

void setupTransmitter() {
  rmt_config_t config = RMT_DEFAULT_CONFIG_TX(IR_TX_PIN, RMT_TX_CH);
  config.clk_div = 80;                             // 1 tick = 1us
  config.tx_config.carrier_en = true;              // dieu che song mang
  config.tx_config.carrier_freq_hz = 38000;        // 38kHz dung tan so mat thu
  config.tx_config.carrier_duty_percent = 33;
  config.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH;
  rmt_config(&config);
  rmt_driver_install(config.channel, 0, 0);
}

void setItem(rmt_item32_t &item, uint32_t highUs, uint32_t lowUs) {
  item.level0 = 1;
  item.duration0 = highUs;
  item.level1 = 0;
  item.duration1 = lowUs;
}

void sendNEC(uint8_t address, uint8_t command) {
  rmt_item32_t items[34];
  uint32_t data = address | ((uint32_t)(uint8_t)~address << 8) |
                  ((uint32_t)command << 16) | ((uint32_t)(uint8_t)~command << 24);

  setItem(items[0], 9000, 4500);                           // header
  for (int i = 0; i < 32; i++) {
    setItem(items[i + 1], 560, (data & (1UL << i)) ? 1690 : 560);
  }
  setItem(items[33], 560, 0);                              // xung ket thuc

  rmt_write_items(RMT_TX_CH, items, 34, true);
  Serial.printf("Da phat NEC: dia chi 0x%02X, lenh 0x%02X\n", address, command);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  setupReceiver();
  setupTransmitter();

  Serial.println("San sang: bam remote de doc ma, bam nut BOOT de phat lenh IR");
}

void loop() {
  checkReceiver();

  static int lastButton = HIGH;
  int button = digitalRead(BUTTON_PIN);
  if (button == LOW && lastButton == HIGH) {
    sendNEC(SEND_ADDRESS, SEND_COMMAND);
    delay(200);
  }
  lastButton = button;
}

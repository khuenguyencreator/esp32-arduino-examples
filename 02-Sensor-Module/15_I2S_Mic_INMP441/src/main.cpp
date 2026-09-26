#include <Arduino.h>

#include <driver/i2s.h>

// Doc am thanh tu micro so INMP441 qua I2S. Micro nay xuat thang du lieu so
// 24 bit, khong can khuech dai hay ADC nhu micro analog (MAX9814, KY-038) -
// it nhieu hon han, day la buoc dau cho cac du an nhan dien giong noi, do
// tieng on, ghi am.
//
// Bai nay tinh muc am luong (RMS) moi 32ms va in ra de xem bang Serial
// Plotter (PlatformIO: cai extension "Serial Plotter", hoac dung Arduino IDE).
//
// Noi day:
//   SCK -> GPIO32   WS -> GPIO25   SD -> GPIO33
//   L/R -> GND (chon kenh trai)    VDD -> 3.3V   GND -> GND

#define I2S_SCK  32
#define I2S_WS   25
#define I2S_SD   33

const int SAMPLE_RATE = 16000;   // 16kHz la du cho giong noi
const int BUFFER_LEN = 512;      // 512 mau / 16kHz = 32ms moi lan tinh

int32_t samples[BUFFER_LEN];

void setupI2S() {
  i2s_config_t config = {};
  config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
  config.sample_rate = SAMPLE_RATE;
  config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;   // INMP441 gui 24 bit trong khung 32 bit
  config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
  config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  config.dma_buf_count = 4;
  config.dma_buf_len = BUFFER_LEN;
  config.use_apll = false;

  i2s_pin_config_t pins = {};
  pins.mck_io_num = I2S_PIN_NO_CHANGE;
  pins.bck_io_num = I2S_SCK;
  pins.ws_io_num = I2S_WS;
  pins.data_out_num = I2S_PIN_NO_CHANGE;
  pins.data_in_num = I2S_SD;

  i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pins);
}

void setup() {
  Serial.begin(115200);
  setupI2S();
}

void loop() {
  size_t bytesRead = 0;
  i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, portMAX_DELAY);
  int count = bytesRead / sizeof(int32_t);
  if (count == 0) return;

  // Du lieu 24 bit nam o phan cao cua so 32 bit -> dich phai 8 bit
  double sumSquares = 0;
  long sum = 0;
  for (int i = 0; i < count; i++) {
    samples[i] >>= 8;
    sum += samples[i];
  }
  long mean = sum / count;   // bo thanh phan mot chieu (DC offset) cua micro
  for (int i = 0; i < count; i++) {
    double v = samples[i] - mean;
    sumSquares += v * v;
  }
  double rms = sqrt(sumSquares / count);

  // Dinh dang "ten:gia_tri" de Serial Plotter ve thanh duong co ten
  Serial.printf("am_luong:%.0f\n", rms);

  // Neu luc nao cung ra 0: chip INMP441 cua ban co the dang gui o kenh phai,
  // doi ONLY_LEFT thanh ONLY_RIGHT (mot so ban ESP32 dao nguoc 2 kenh).
}

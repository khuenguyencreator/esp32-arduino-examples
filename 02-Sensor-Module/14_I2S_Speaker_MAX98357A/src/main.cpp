#include <Arduino.h>

#include <driver/i2s.h>

// Phat am thanh ra loa qua module MAX98357A - mach khuech dai class D nhan
// thang tin hieu so I2S, khong can DAC hay mach khuech dai rieng. Khac bai
// 01-Basic/04_DAC dung DAC 8 bit cua ESP32 (tieng re, nhieu), I2S cho chat
// luong 16 bit 44.1kHz nhu dia CD.
//
// Bai nay tu tao song sin de choi thang am Do - Re - Mi - Fa - Sol - La - Si - Do.
//
// Noi day:
//   BCLK -> GPIO26   LRC -> GPIO25   DIN -> GPIO22
//   VIN  -> 5V       GND -> GND      loa 4-8 ohm noi vao 2 cham +/- cua module
//   Chan SD va GAIN de trong (mac dinh: bat, tron 2 kenh, khuech dai 9dB)

#define I2S_BCLK  26
#define I2S_LRC   25
#define I2S_DOUT  22

const int SAMPLE_RATE = 44100;
const float VOLUME = 0.2;   // 0.0 - 1.0, de nho de khong be tieng loa nho

// Tan so (Hz) cac not nhac quang tam 4
const float NOTES[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
const char* NOTE_NAMES[] = {"Do", "Re", "Mi", "Fa", "Sol", "La", "Si", "Do"};

void setupI2S() {
  i2s_config_t config = {};
  config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  config.sample_rate = SAMPLE_RATE;
  config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;       // stereo
  config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  config.dma_buf_count = 8;
  config.dma_buf_len = 64;
  config.use_apll = false;
  config.tx_desc_auto_clear = true;   // het du lieu thi tu phat im lang, khong bi re

  i2s_pin_config_t pins = {};
  pins.mck_io_num = I2S_PIN_NO_CHANGE;   // MAX98357A khong can xung MCLK
  pins.bck_io_num = I2S_BCLK;
  pins.ws_io_num = I2S_LRC;
  pins.data_out_num = I2S_DOUT;
  pins.data_in_num = I2S_PIN_NO_CHANGE;

  i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pins);
}

void playTone(float frequency, int durationMs) {
  const int FRAMES = 256;
  int16_t buffer[FRAMES * 2];   // moi frame gom 2 mau: trai + phai
  static float phase = 0;
  const float phaseStep = 2 * PI * frequency / SAMPLE_RATE;

  long totalFrames = (long)SAMPLE_RATE * durationMs / 1000;
  while (totalFrames > 0) {
    int frames = min((long)FRAMES, totalFrames);
    for (int i = 0; i < frames; i++) {
      int16_t sample = frequency > 0 ? (int16_t)(sin(phase) * 32767 * VOLUME) : 0;
      buffer[i * 2] = sample;       // kenh trai
      buffer[i * 2 + 1] = sample;   // kenh phai
      phase += phaseStep;
      if (phase > 2 * PI) phase -= 2 * PI;
    }
    size_t written;
    // Ham nay tu cho khi DMA con cho trong, nen vong lap chay dung toc do phat
    i2s_write(I2S_NUM_0, buffer, frames * 2 * sizeof(int16_t), &written, portMAX_DELAY);
    totalFrames -= frames;
  }
}

void setup() {
  Serial.begin(115200);
  setupI2S();
  Serial.println("I2S san sang, choi thang am");
}

void loop() {
  for (int i = 0; i < 8; i++) {
    Serial.println(NOTE_NAMES[i]);
    playTone(NOTES[i], 400);
    playTone(0, 50);   // tan so 0 = phat mau 0 (im lang) ngan giua 2 not
  }
  delay(1000);
}

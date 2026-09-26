#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// May nghe nhac MP3: doc file MP3 tu the SD, giai ma bang phan mem (thu vien
// ESP8266Audio, chay tot tren ESP32) roi phat ra loa qua MAX98357A.
// Ghep 2 bai 12_SD_Card va 14_I2S_Speaker_MAX98357A.
//
// Chuan bi: tao thu muc /music tren the SD, chep vai file .mp3 vao (nen de
// bitrate 128kbps, 44.1kHz - file bitrate cao qua ESP32 giai ma khong kip).
// Bam nut BOOT (GPIO0) de chuyen bai.
//
// Noi day:
//   The SD:     CS -> GPIO5, SCK -> 18, MOSI -> 23, MISO -> 19
//   MAX98357A:  BCLK -> GPIO26, LRC -> GPIO25, DIN -> GPIO22

const int SD_CS = 5;
const int BUTTON_PIN = 0;
const char* MUSIC_DIR = "/music";
const int MAX_TRACKS = 50;

String playlist[MAX_TRACKS];
int trackCount = 0;
int currentTrack = 0;

AudioOutputI2S *out = nullptr;
AudioGeneratorMP3 *mp3 = nullptr;
AudioFileSourceSD *file = nullptr;

void loadPlaylist() {
  File dir = SD.open(MUSIC_DIR);
  File entry = dir.openNextFile();
  while (entry && trackCount < MAX_TRACKS) {
    String name = entry.name();
    if (!entry.isDirectory() && name.endsWith(".mp3")) {
      playlist[trackCount++] = String(MUSIC_DIR) + "/" + name;
    }
    entry = dir.openNextFile();
  }
  Serial.printf("Tim thay %d bai trong %s\n", trackCount, MUSIC_DIR);
}

void playTrack(int index) {
  if (mp3 && mp3->isRunning()) mp3->stop();
  delete file;

  file = new AudioFileSourceSD(playlist[index].c_str());
  mp3->begin(file, out);
  Serial.printf("Dang phat [%d/%d]: %s\n", index + 1, trackCount, playlist[index].c_str());
}

void nextTrack() {
  currentTrack = (currentTrack + 1) % trackCount;
  playTrack(currentTrack);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!SD.begin(SD_CS)) {
    Serial.println("Khong khoi tao duoc the SD");
    while (true) delay(1000);
  }

  loadPlaylist();
  if (trackCount == 0) {
    Serial.println("Khong co file .mp3 nao trong /music");
    while (true) delay(1000);
  }

  out = new AudioOutputI2S();
  out->SetPinout(26, 25, 22);   // BCLK, LRC, DIN
  out->SetGain(0.3);            // am luong 0.0 - 4.0

  mp3 = new AudioGeneratorMP3();
  playTrack(currentTrack);
}

void loop() {
  // loop() phai goi mp3->loop() lien tuc de day du lieu xuong I2S,
  // KHONG dung delay() dai o day, nhac se bi giat.
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      nextTrack();   // het bai tu chuyen bai tiep
    }
  }

  static int lastButton = HIGH;
  int button = digitalRead(BUTTON_PIN);
  if (button == LOW && lastButton == HIGH) {
    nextTrack();
  }
  lastButton = button;
}

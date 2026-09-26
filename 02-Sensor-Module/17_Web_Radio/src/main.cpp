#include <Arduino.h>

#include <WiFi.h>
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// Radio internet: ESP32 ket noi WiFi, nhan luong MP3 tu 1 dai radio online
// roi phat ra loa qua MAX98357A. Giong bai 16_MP3_Player_SD, chi doi nguon
// du lieu tu file tren the SD sang luong HTTP.
//
// Luu y: thu vien chi doc duoc luong MP3 qua http:// (khong phai https://,
// khong phai HLS .m3u8). Dai trong vi du co the doi dia chi theo thoi gian,
// neu khong phat duoc hay tim 1 dai MP3 http khac thay vao STREAM_URL.
//
// Noi MAX98357A: BCLK -> GPIO26, LRC -> GPIO25, DIN -> GPIO22

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

const char* STREAM_URL = "http://stream.srg-ssr.ch/m/rsj/mp3_128";   // Radio Swiss Jazz

AudioGeneratorMP3 *mp3 = nullptr;
AudioFileSourceICYStream *stream = nullptr;
AudioFileSourceBuffer *buffer = nullptr;
AudioOutputI2S *out = nullptr;

// Dai radio gui kem ten bai hat dang phat (metadata ICY) - in ra cho vui
void onMetadata(void *cbData, const char *type, bool isUnicode, const char *value) {
  Serial.printf("[%s] %s\n", type, value);
}

void startStream() {
  if (mp3) { mp3->stop(); delete mp3; }
  delete buffer;
  delete stream;

  stream = new AudioFileSourceICYStream(STREAM_URL);
  stream->RegisterMetadataCB(onMetadata, NULL);

  // Bo dem 16KB: WiFi nhan du lieu theo tung dot khong deu, bo dem giup
  // nhac khong bi ngat quang moi khi mang cham lai 1 chut
  buffer = new AudioFileSourceBuffer(stream, 16 * 1024);

  mp3 = new AudioGeneratorMP3();
  mp3->begin(buffer, out);
  Serial.println("Dang phat radio...");
}

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  out = new AudioOutputI2S();
  out->SetPinout(26, 25, 22);
  out->SetGain(0.3);

  startStream();
}

void loop() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
    }
  } else {
    // Mat ket noi hoac dai ngat luong -> cho 3 giay roi ket noi lai
    Serial.println("Luong bi ngat, ket noi lai sau 3 giay");
    delay(3000);
    startStream();
  }
}

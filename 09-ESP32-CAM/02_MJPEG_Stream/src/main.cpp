#include <Arduino.h>

#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// Bai nay noi tiep 09-ESP32-CAM/01_Take_Photo_SD (chup anh luu the nho) - thay vi chup 1
// tam roi luu, ESP32 se lien tuc chup va gui ve trinh duyet duoi dang
// video MJPEG (chuoi anh JPEG lien tuc) qua webserver, xem duoc truc tiep
// tren trinh duyet nhu 1 camera giam sat mini.

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

WebServer server(80);

// Ranh giới giữa các khung hình JPEG liên tiếp trong luồng MJPEG
static const char *STREAM_BOUNDARY = "\r\n--frame\r\n";
static const char *STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = psramFound() ? 2 : 1;

  return esp_camera_init(&config) == ESP_OK;
}

// Luu y: handleStream() chay vong lap gui anh lien tuc va chiem luon loop(),
// nen moi luc chi 1 trinh duyet xem duoc stream - trinh duyet thu 2 phai cho
// trinh duyet dau dong tab.
void handleStream() {
  WiFiClient client = server.client();

  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  client.print(response);

  while (client.connected()) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Chup khung hinh that bai");
      break;
    }

    client.print(STREAM_BOUNDARY);

    char header[64];
    size_t headerLen = snprintf(header, sizeof(header), STREAM_PART, fb->len);
    client.write(header, headerLen);
    client.write(fb->buf, fb->len);

    esp_camera_fb_return(fb);

    if (!client.connected()) {
      break;
    }
  }
}

void handleRoot() {
  String html = "<html><body><h1>ESP32-CAM Stream</h1>";
  html += "<img src=\"/stream\" style=\"width:100%;max-width:640px;\">";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  if (!initCamera()) {
    Serial.println("Khoi tao camera that bai");
    return;
  }

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("Mo trinh duyet toi: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/stream", handleStream);
  server.begin();
}

void loop() {
  server.handleClient();
}

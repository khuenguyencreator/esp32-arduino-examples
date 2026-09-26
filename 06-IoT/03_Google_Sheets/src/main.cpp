#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Google Sheets khong co san REST API don gian cho thiet bi nhung, nhung
// Google Apps Script cho phep tao 1 "Web App" - 1 doan script chay tren
// Google, nhan request HTTP roi tu ghi vao Sheet giup minh. ESP32 chi can
// goi HTTPS toi dung URL Web App do la xong, khong can OAuth phuc tap.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

// URL Web App sau khi Deploy script Google Apps Script (dang .../exec)
const char* scriptUrl = "https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec";

void logToGoogleSheet(float temperature, float humidity) {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;

  String url = String(scriptUrl) + "?temperature=" + String(temperature) +
               "&humidity=" + String(humidity);

  if (https.begin(client, url)) {
    // Apps Script tra ve 302 (chuyen huong sang googleusercontent.com) sau khi
    // ghi xong - bat theo chuyen huong de nhan duoc ma 200 va noi dung tra ve.
    https.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = https.GET();

    if (httpCode > 0) {
      Serial.print("Ma phan hoi Google Sheets: ");
      Serial.println(httpCode);
      Serial.println(https.getString());
    } else {
      Serial.print("Loi khi goi Google Sheets: ");
      Serial.println(https.errorToString(httpCode));
    }
    https.end();
  } else {
    Serial.println("Khong the ket noi toi Google Apps Script");
  }
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
  Serial.println("");
  Serial.println("WiFi connected.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = random(200, 350) / 10.0;
    float humidity = random(400, 800) / 10.0;

    Serial.println("Dang ghi du lieu vao Google Sheet...");
    logToGoogleSheet(temperature, humidity);
  }

  delay(30000); // Google Apps Script gioi han so luot goi/phut, khong nen goi qua day
}

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <base64.h>

// Gui email khong can thu vien ngoai cong kenh - Gmail (va hau het dich vu
// email) ho tro giao thuc SMTP qua ket noi TLS ngay tren cong 465. ESP32 chi
// can "noi chuyen" dung cu phap SMTP (EHLO, AUTH LOGIN, MAIL FROM, RCPT TO,
// DATA...) qua WiFiClientSecure la gui duoc email, khong can thu vien rieng.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

const char* smtpServer = "smtp.gmail.com";
const int smtpPort = 465;

// Dung Mat khau ung dung (App Password) cua Gmail, KHONG dung mat khau dang nhap thuong
const char* emailSender = "email_gui_cua_ban@gmail.com";
const char* emailPassword = "app_password_16_ky_tu";
const char* emailReceiver = "email_nhan@gmail.com";

WiFiClientSecure client;

bool waitResponse() {
  unsigned long start = millis();
  while (!client.available() && millis() - start < 5000) {
    delay(10);
  }

  String response = "";
  while (client.available()) {
    response += (char)client.read();
  }

  Serial.print(response);
  return response.startsWith("2") || response.startsWith("3");
}

bool sendEmail(String subject, String message) {
  if (!client.connect(smtpServer, smtpPort)) {
    Serial.println("Khong ket noi duoc toi SMTP server");
    return false;
  }

  waitResponse();

  client.println("EHLO esp32.local");
  waitResponse();

  client.println("AUTH LOGIN");
  waitResponse();

  client.println(base64::encode(emailSender));
  waitResponse();

  client.println(base64::encode(emailPassword));
  if (!waitResponse()) {
    Serial.println("Dang nhap that bai - kiem tra lai App Password");
    client.stop();
    return false;
  }

  client.println("MAIL FROM: <" + String(emailSender) + ">");
  waitResponse();

  client.println("RCPT TO: <" + String(emailReceiver) + ">");
  waitResponse();

  client.println("DATA");
  waitResponse();

  client.println("From: ESP32 <" + String(emailSender) + ">");
  client.println("To: <" + String(emailReceiver) + ">");
  client.println("Subject: " + subject);
  client.println();
  client.println(message);
  client.println(".");
  bool ok = waitResponse();

  client.println("QUIT");
  client.stop();

  return ok;
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

  Serial.println("Dang gui email canh bao...");
  if (sendEmail("ESP32 canh bao", "Nhiet do vuot nguong cho phep!")) {
    Serial.println("Gui email thanh cong");
  } else {
    Serial.println("Gui email that bai");
  }
}

void loop() {
  // Bai nay chi gui 1 lan de demo trong setup(), thuc te ban nen goi
  // sendEmail() khi co su kien can canh bao (vd: nhiet do vuot nguong)
}

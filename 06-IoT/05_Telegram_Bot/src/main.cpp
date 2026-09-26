#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Telegram Bot API la 1 REST API HTTPS binh thuong - khong can thu vien rieng,
// chi can HTTPClient nhu cac bai truoc. Bai nay lam 2 chieu: ESP32 dinh ky
// hoi Telegram "co tin nhan moi khong" (long polling qua getUpdates), nhan
// lenh dieu khien LED, va gui tin nhan phan hoi lai qua sendMessage.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

// Lay Bot Token tu @BotFather, Chat ID tu @userinfobot tren Telegram
const char* botToken = "123456789:AAExampleBotTokenFromBotFather";
const char* chatId = "123456789";

const int led = 26;
long lastUpdateId = 0;

// Ma hoa noi dung tin nhan truoc khi gan len URL (dau cach, dau tieng Viet,
// ky tu dac biet...) - neu khong, request se bi hong va Telegram khong nhan.
String urlEncode(const String &text) {
  String encoded = "";
  char buf[4];
  for (size_t i = 0; i < text.length(); i++) {
    char c = text[i];
    if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else {
      snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)c);
      encoded += buf;
    }
  }
  return encoded;
}

void sendTelegramMessage(String text) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  String url = "https://api.telegram.org/bot" + String(botToken) +
               "/sendMessage?chat_id=" + String(chatId) + "&text=" + urlEncode(text);

  if (https.begin(client, url)) {
    https.GET();
    https.end();
  }
}

void checkTelegramMessages() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  String url = "https://api.telegram.org/bot" + String(botToken) +
               "/getUpdates?offset=" + String(lastUpdateId + 1) + "&timeout=10";

  if (https.begin(client, url)) {
    // Telegram giu ket noi toi 10 giay (timeout=10) cho tin nhan moi, nen
    // phai tang timeout cua HTTPClient (mac dinh 5 giay) len lau hon muc do.
    https.setTimeout(15000);
    int httpCode = https.GET();

    if (httpCode == 200) {
      String payload = https.getString();

      DynamicJsonDocument doc(4096);
      deserializeJson(doc, payload);

      JsonArray results = doc["result"].as<JsonArray>();
      for (JsonObject update : results) {
        lastUpdateId = update["update_id"].as<long>();
        String text = update["message"]["text"].as<String>();

        Serial.print("Nhan lenh Telegram: ");
        Serial.println(text);

        if (text == "/led_on") {
          digitalWrite(led, HIGH);
          sendTelegramMessage("Da bat LED");
        } else if (text == "/led_off") {
          digitalWrite(led, LOW);
          sendTelegramMessage("Da tat LED");
        } else if (text == "/status") {
          sendTelegramMessage(digitalRead(led) ? "LED dang bat" : "LED dang tat");
        }
      }
    }
    https.end();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  sendTelegramMessage("ESP32 da khoi dong, san sang nhan lenh /led_on, /led_off, /status");
}

void loop() {
  checkTelegramMessages();
  delay(1000);
}

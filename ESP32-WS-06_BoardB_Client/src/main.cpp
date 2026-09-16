#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>

// Board B dong vai tro Client - doc nut bam cuc bo, gui lenh dieu khien Led
// sang Board A qua HTTP, dong thoi dinh ky doc trang thai nut bam cua Board A
// ve de "phan chieu" len led cua chinh minh.

const int led = 26;
const int button = 4;

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

// Thay bang dia chi IP that cua Board A - xem trong Serial Monitor cua Board A
// sau khi nap code va board da ket noi wifi thanh cong
const char* boardA_IP = "192.168.1.100";

bool ledOnBoardA = false;
int lastButtonState = HIGH;
unsigned long lastPollTime = 0;
const unsigned long pollInterval = 1000; // 1 giay doc trang thai Board A 1 lan

String httpGETRequest(const char* Url);

void setup() {
  Serial.begin(115200);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(button, INPUT_PULLUP);

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address cua Board B (Client): ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 1. Doc nut bam cuc bo, neu vua nhan thi gui lenh dao trang thai led sang Board A
  int buttonState = digitalRead(button);
  if (buttonState == LOW && lastButtonState == HIGH)
  {
    ledOnBoardA = !ledOnBoardA;
    String path = ledOnBoardA ? "/led/on" : "/led/off";
    String url = "http://" + String(boardA_IP) + path;
    Serial.println("Nut bam! Goi sang Board A: " + url);
    httpGETRequest(url.c_str());
    delay(200); // debounce don gian
  }
  lastButtonState = buttonState;

  // 2. Dinh ky doc trang thai nut bam cua Board A, phan chieu len led cua Board B
  if (millis() - lastPollTime >= pollInterval)
  {
    lastPollTime = millis();
    String url = "http://" + String(boardA_IP) + "/status";
    String status = httpGETRequest(url.c_str());
    Serial.println("Trang thai Board A: " + status);

    if (status.indexOf("BUTTON=PRESSED") >= 0)
    {
      digitalWrite(led, HIGH);
    }
    else if (status.indexOf("BUTTON=RELEASED") >= 0)
    {
      digitalWrite(led, LOW);
    }
  }
}

String httpGETRequest(const char* Url)
{
  HTTPClient http;
  http.begin(Url);
  int responseCode = http.GET();
  String responseBody = "";
  if (responseCode > 0)
  {
    Serial.print("responseCode: ");
    Serial.println(responseCode);
    responseBody = http.getString();
  }
  else
  {
    Serial.print("Error Code: ");
    Serial.println(responseCode);
  }
  http.end();
  return responseBody;
}

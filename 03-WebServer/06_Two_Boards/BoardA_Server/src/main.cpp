#include <Arduino.h>

#include <WiFi.h>

// Board A dong vai tro Webserver - vua nhan lenh dieu khien Led tu Board B gui toi,
// vua bao cao trang thai nut bam cuc bo de Board B doc ve.

// khai bao chan su dung
const int led = 26;
const int button = 4; // nut bam cuc bo, mot chan noi GND, dung INPUT_PULLUP

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

// Tao mot web server tai cong 80 - cong mac dinh cho web
WiFiServer webServer(80);

String ledStatus = "OFF";
String header;

unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

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
  Serial.print("IP address cua Board A (Server): ");
  Serial.println(WiFi.localIP());

  // khoi tao webserver
  webServer.begin();
}

void loop() {
  WiFiClient webClient = webServer.available();

  if (webClient)
  {
    // khoi tao gia tri ban dau cho time
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New client ket noi toi Board A");
    // bien luu gia tri response
    String currentLine = "";
    // neu co client connect va khong qua thoi gian time out
    while (webClient.connected() && currentTime - previousTime <= timeoutTime)
    {
      // doc gia tri timer tai thoi diem hien tai
      currentTime = millis();
      // neu client con ket noi
      if (webClient.available())
      {
        // doc gia tri truyen tu client theo tung byte kieu char
        char c = webClient.read();
        Serial.write(c);
        header += c; // luu gia tri vao Header
        if (c == '\n') // Neu doc duoc ki tu xuong dong (het chuoi truyen toi)
        {
          if (currentLine.length() == 0)
          {
            // Xu ly lenh dieu khien Led truoc, roi moi tra loi trang thai
            if (header.indexOf("GET /led/on") >= 0)
            {
              digitalWrite(led, HIGH);
              ledStatus = "ON";
              Serial.println("Board A: bat led theo lenh tu Board B");
            }
            else if (header.indexOf("GET /led/off") >= 0)
            {
              digitalWrite(led, LOW);
              ledStatus = "OFF";
              Serial.println("Board A: tat led theo lenh tu Board B");
            }
            // route /status khong lam gi them, chi tra ve trang thai hien tai

            // doc trang thai nut bam cuc bo, gui kem trong moi response
            String buttonStatus = (digitalRead(button) == LOW) ? "PRESSED" : "RELEASED";

            // HTTP headers luon luon bat dau voi code HTTP (vi du HTTP/1.1 200 OK)
            webClient.println("HTTP/1.1 200 OK");
            webClient.println("Content-type:text/plain"); // tra ve plain text de Board B de doc, khong can HTML
            webClient.println("Connection: close"); // dong ket noi sau khi tra loi
            webClient.println();
            // Response dang 1 dong ngan gon, vi du: LED=ON;BUTTON=RELEASED
            webClient.println("LED=" + ledStatus + ";BUTTON=" + buttonStatus);
            // Break out of the while loop
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r') // neu gia tri gui toi khac xuong dong
        {
          currentLine += c; // luu gia tri vao bien
        }
      }
    }
    // Xoa header de su dung cho lan toi
    header = "";
    // ngat ket noi
    webClient.stop();
    Serial.println("Client ngat ket noi.");
    Serial.println("");
  }
}

#include <Arduino.h>

#include <WiFi.h>
#include <SPI.h>
#include <SD.h>
#include <time.h>

// Bo ghi du lieu (datalogger): cu 10 giay doc 1 gia tri cam bien, ghi vao
// file CSV tren the SD kem moc thoi gian that lay tu NTP. File CSV mo thang
// bang Excel/Google Sheets de ve bieu do.
//
// Ghep 2 bai da hoc: 12_SD_Card (ghi file) va 07-OTA-WiFiConfig/02_WiFiManager_NTP
// (lay gio NTP). Cam bien o day la bien tro noi GPIO34 cho de lam, thay bang
// cam bien that (DHT22, BME280...) chi can sua ham readSensor().
//
// Noi the SD giong bai 12_SD_Card: CS -> GPIO5, SCK -> 18, MOSI -> 23, MISO -> 19.

const char* ssid = "Ten_Wifi_Cua_Ban";
const char* password = "Mat_Khau_Wifi";

const char* ntpServer = "pool.ntp.org";
const long gmtOffsetSec = 7 * 3600;   // GMT+7

const int SD_CS = 5;
const int SENSOR_PIN = 34;
const char* LOG_FILE = "/datalog.csv";
const unsigned long LOG_INTERVAL = 10000;

float readSensor() {
  int raw = analogRead(SENSOR_PIN);
  return raw * 3.3 / 4095.0;   // doi gia tri ADC ra dien ap (V)
}

void setup() {
  Serial.begin(115200);

  if (!SD.begin(SD_CS)) {
    Serial.println("Khong khoi tao duoc the SD");
    while (true) delay(1000);
  }

  // Tao file va dong tieu de CSV neu chua co
  if (!SD.exists(LOG_FILE)) {
    File file = SD.open(LOG_FILE, FILE_WRITE);
    file.println("thoi_gian,dien_ap_V");
    file.close();
  }

  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  configTime(gmtOffsetSec, 0, ntpServer);

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Dang cho dong bo gio NTP...");
    delay(1000);
  }
  Serial.println("Da co gio chuan, bat dau ghi du lieu");
}

void loop() {
  static unsigned long lastLog = 0;
  if (millis() - lastLog < LOG_INTERVAL) return;
  lastLog = millis();

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Mat gio NTP, bo qua lan ghi nay");
    return;
  }

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  float value = readSensor();

  // Mo - ghi - dong ngay trong moi lan ghi: neu mat dien dot ngot thi chi mat
  // toi da 1 dong dang ghi, khong hong ca file nhu khi de file mo lien tuc.
  File file = SD.open(LOG_FILE, FILE_APPEND);
  if (!file) {
    Serial.println("Loi mo file log");
    return;
  }
  file.printf("%s,%.3f\n", timeString, value);
  file.close();

  Serial.printf("Da ghi: %s, %.3f V\n", timeString, value);
}

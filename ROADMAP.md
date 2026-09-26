# Kế hoạch bài viết

Danh sách các bài sẽ bổ sung vào repo, xếp theo đúng nhóm thư mục. Bài nào xong sẽ chuyển lên [README](README.md).

Quy ước:
- Bài dùng **module gắn ngoài** ESP32 (cảm biến, màn hình, thẻ nhớ, âm thanh, động cơ…) đều nằm trong `02-Sensor-Module`.
- Bài dùng **ngoại vi có sẵn trong chip** nằm trong `01-Basic`.
- ⭐ = nên làm trước.

## Đã hoàn thành trong đợt này

| Nhóm | Bài |
|---|---|
| 01-Basic | `12_Hall_Sensor_Internal_Temp`, `13_RMT` (WS2812 + IR NEC), `14_LittleFS` |
| 02-Sensor-Module | `06_LCD1602_I2C`, `07_RTC_DS3231`, `08_RFID_RC522`, `09_Keypad_4x4`, `10_Servo`, `11_Stepper_A4988`, `12_SD_Card`, `13_SD_Datalogger_NTP`, `14_I2S_Speaker_MAX98357A`, `15_I2S_Mic_INMP441`, `16_MP3_Player_SD`, `17_Web_Radio`, `18_TFT_eSPI`, `19_LVGL` |
| 03-WebServer | `07_mDNS_StaticIP_Reconnect` |
| 04-Protocol | `09_Modbus_RTU_RS485` |
| 07-OTA-WiFiConfig | `05_ArduinoOTA_PlatformIO` |

## Sắp làm

### 01-Basic
Số 09–11 đang để trống, dành cho 3 giao thức nối tiếp cơ bản (nên học trước các bài module):
- ⭐ `09_UART`: Serial2, đổi chân, giao tiếp với module khác
- ⭐ `10_I2C_Master`: tự đọc/ghi thanh ghi bằng `Wire`, không dùng thư viện
- ⭐ `11_SPI`: SPI master cơ bản
- `15_Watchdog`: Task WDT, đọc lỗi "Guru Meditation"

### 02-Sensor-Module
- ⭐ DS18B20 (1-Wire), ⭐ DHT11/DHT22
- Cảm biến siêu âm HC-SR04, cảm biến chuyển động PIR
- HX711 (loadcell), INA219 (đo dòng/áp)
- Rotary encoder, relay

### 03-WebServer
- ⭐ ESPAsyncWebServer + REST API trả JSON (GET/POST)
- Web server có đăng nhập (Basic Auth)

### 04-Protocol
- ⭐ MQTT nâng cao: TLS, username/password, QoS, retain, Last Will
- CAN bus (TWAI) với SN65HVD230
- Ethernet (LAN8720 / W5500), LoRa SX1278

### 05-Bluetooth
- ⭐ BLE Notify: server chủ động đẩy dữ liệu cảm biến
- BLE HID (ESP32 làm bàn phím/chuột), iBeacon

### 06-IoT
- ⭐ Home Assistant (MQTT Discovery hoặc ESPHome)
- Blynk IoT, AWS IoT Core / ThingsBoard

### 07-OTA-WiFiConfig
- ⭐ Lưu cấu hình bằng NVS / Preferences
- Bảng phân vùng (partition table) và rollback firmware khi OTA lỗi

### 08-FreeRTOS
- ⭐ Queue: truyền dữ liệu giữa các task
- ⭐ Semaphore và Mutex: dùng chung Serial/I2C an toàn
- Event Group, Software Timer
- Đánh thức task từ ngắt (ISR → task notification)

### 09-ESP32-CAM
- Phát hiện chuyển động (PIR) → chụp ảnh gửi Telegram
- Nhận diện khuôn mặt với ESP-WHO

### Nhóm mới
Khi tạo các nhóm này, `10-Projects` sẽ được dời xuống cuối để dự án luôn là phần tổng hợp sau cùng.
- **Power**: Light sleep vs Deep sleep, đo dòng tiêu thụ thực tế, ULP coprocessor
- **Debug-Tools**: ⭐ exception decoder, debug JTAG với ESP-Prog, core dump, unit test PlatformIO
- **ESP32-Variants**: ESP32-S3, C3, C6 – so sánh và cách chọn chip

## Không nằm trong repo này
- **ESP-IDF** (chuyển từ Arduino sang ESP-IDF): sẽ làm thành một series và repo riêng.

## Thứ tự ưu tiên đề xuất
1. UART
2. I2C master
3. SPI
4. DS18B20 + DHT22
5. NVS / Preferences
6. FreeRTOS Queue
7. FreeRTOS Mutex / Semaphore
8. MQTT nâng cao
9. Home Assistant
10. Exception decoder / debug crash

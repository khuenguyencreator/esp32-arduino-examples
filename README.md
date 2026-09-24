# ESP32 Arduino Examples

Code ví dụ lập trình **ESP32** với **Arduino framework** trên **PlatformIO**, từ ngoại vi cơ bản (GPIO, ADC, PWM, Timer…) đến WiFi, Web Server, BLE, IoT và các dự án hoàn chỉnh. Code đi kèm các bài viết trên blog [khuenguyencreator.com](https://khuenguyencreator.com).

- Board: ESP32 DOIT DevKit V1 (`esp32doit-devkit-v1`); riêng các bài `ESP32-CAM-*` dùng board **ESP32-CAM** (`esp32cam`)
- Framework: Arduino (`platform = espressif32`)
- IDE: VS Code + PlatformIO
- Serial Monitor: 115200 baud

## Danh sách bài

### Ngoại vi cơ bản
| Project | Nội dung |
|---|---|
| [Bai_1_ESP32_GPIO](Bai_1_ESP32_GPIO) | GPIO Input / Output |
| [Bai_2_ESP32_ADC](Bai_2_ESP32_ADC) | Đọc ADC |
| [Bai_3_ESP32_PWM](Bai_3_ESP32_PWM) | Băm xung PWM (LEDC) |
| [Bai_4_ESP32_DAC](Bai_4_ESP32_DAC) | Xuất DAC |
| [Bai_5_External_Interrupts](Bai_5_External_Interrupts) | Ngắt ngoài |
| [Bai_6_1_ESP32_Timer_Millis](Bai_6_1_ESP32_Timer_Millis) | Định thời bằng `millis()` |
| [Bai_6_2_ESP32_Timer_Interrupts](Bai_6_2_ESP32_Timer_Interrupts) | Ngắt Timer phần cứng |
| [Bai_7_ESP32_Touch_Pin](Bai_7_ESP32_Touch_Pin) | Cảm biến chạm (Touch Pin) |
| [Bai_8_1_ESP32_Timer_WakeUP](Bai_8_1_ESP32_Timer_WakeUP) | Deep Sleep – đánh thức bằng Timer |
| [Bai_8_2_ESP32_Touch_WakeUP](Bai_8_2_ESP32_Touch_WakeUP) | Deep Sleep – đánh thức bằng Touch |
| [Bai_8_3_ESP32_EXT_WakeUP](Bai_8_3_ESP32_EXT_WakeUP) | Deep Sleep – đánh thức bằng chân ngoài |

### Cảm biến & màn hình I2C
| Project | Nội dung |
|---|---|
| [ESP32_I2C_Scan](ESP32_I2C_Scan) | Quét địa chỉ thiết bị I2C |
| [ESP32_I2C_SH1106](ESP32_I2C_SH1106) | Màn hình OLED SH1106 |
| [ESP32_APDS9960](ESP32_APDS9960) | Cảm biến cử chỉ / màu APDS9960 |

### Mạng: Web Server, HTTP, MQTT, OTA
| Project | Nội dung |
|---|---|
| [Net_Bai_1_ESP32_Webserver_Station](Net_Bai_1_ESP32_Webserver_Station) | Web Server ở chế độ Station |
| [Net_Bai_2_ESP32_Webserver_Access_Point](Net_Bai_2_ESP32_Webserver_Access_Point) | Web Server ở chế độ Access Point |
| [Net_Bai_3_ESP32_Websocket](Net_Bai_3_ESP32_Websocket) | WebSocket |
| [Net_Bai_4_ESP32_HTTP_Openweather](Net_Bai_4_ESP32_HTTP_Openweather) | HTTP GET – lấy thời tiết từ OpenWeather |
| [Net_Bai_5_ESP32_HTTP_Thingspeak](Net_Bai_5_ESP32_HTTP_Thingspeak) | HTTP – gửi dữ liệu lên ThingSpeak |
| [Net_Bai_6_ESP32_MQTT](Net_Bai_6_ESP32_MQTT) | MQTT |
| [Net_Bai_7_ESP32_Smartconfig](Net_Bai_7_ESP32_Smartconfig) | Cấu hình WiFi bằng SmartConfig |
| [Net_Bai_8_ESP32_OTA](Net_Bai_8_ESP32_OTA) | Nạp firmware qua mạng LAN (OTA) |
| [Net_Bai_9_ESP32_FOTA_Internet_OTA_Driver](Net_Bai_9_ESP32_FOTA_Internet_OTA_Driver) | Cập nhật firmware qua Internet (FOTA) |
| [ESP32-UTIL-01_WiFiManager_NTP](ESP32-UTIL-01_WiFiManager_NTP) | WiFiManager + lấy giờ NTP |

### Giao thức truyền thông
| Project | Nội dung |
|---|---|
| [ESP32-PROTO-04_HTTPS](ESP32-PROTO-04_HTTPS) | HTTPS |
| [ESP32-PROTO-05_TCP_Socket](ESP32-PROTO-05_TCP_Socket) | TCP Socket |
| [ESP32-PROTO-06_UDP](ESP32-PROTO-06_UDP) | UDP |
| [ESP32-PROTO-07_WiFi_Mesh](ESP32-PROTO-07_WiFi_Mesh) | WiFi Mesh |
| [ESP32-PROTO-08_ESPNOW_Sender](ESP32-PROTO-08_ESPNOW_Sender) / [Receiver](ESP32-PROTO-08_ESPNOW_Receiver) | ESP-NOW (gửi / nhận) |

### Web Server, SPIFFS & cảm biến
| Project | Nội dung |
|---|---|
| [ESP32-WS-04_SPIFFS_Webserver](ESP32-WS-04_SPIFFS_Webserver) | Web Server lưu giao diện trên SPIFFS |
| [ESP32-WS-05_SPIFFS_Data_Storage](ESP32-WS-05_SPIFFS_Data_Storage) | Lưu dữ liệu vào SPIFFS |
| [ESP32-WS-06_BoardA_Server](ESP32-WS-06_BoardA_Server) / [BoardB_Client](ESP32-WS-06_BoardB_Client) | Giao tiếp 2 board ESP32 (Server / Client) |
| [ESP32-WS-07_SSD1306_BME280](ESP32-WS-07_SSD1306_BME280) | Đọc cảm biến BME280, hiển thị lên OLED SSD1306 |
| [ESP32-WS-08_MPU6050](ESP32-WS-08_MPU6050) | Đọc cảm biến gia tốc / con quay MPU6050 |

### BLE
| Project | Nội dung |
|---|---|
| [ESP32-BLE-01_Classic](ESP32-BLE-01_Classic) | Bluetooth Classic |
| [ESP32-BLE-02_Client](ESP32-BLE-02_Client) | BLE Client |
| [ESP32-BLE-03_Server](ESP32-BLE-03_Server) | BLE Server |

### IoT & Cloud
| Project | Nội dung |
|---|---|
| [ESP32-IOT-01_Firebase](ESP32-IOT-01_Firebase) | Firebase |
| [ESP32-IOT-02_NodeRED](ESP32-IOT-02_NodeRED) | Node-RED |
| [ESP32-IOT-03_Google_Sheets](ESP32-IOT-03_Google_Sheets) | Ghi dữ liệu lên Google Sheets |
| [ESP32-IOT-04_Send_Email](ESP32-IOT-04_Send_Email) | Gửi Email |
| [ESP32-IOT-05_Telegram_Bot](ESP32-IOT-05_Telegram_Bot) | Điều khiển qua Telegram Bot |

### FreeRTOS & ESP32-CAM
| Project | Nội dung |
|---|---|
| [ESP32-RTOS-01_FreeRTOS](ESP32-RTOS-01_FreeRTOS) | FreeRTOS trên ESP32 |
| [ESP32-CAM-01_Take_Photo_SD](ESP32-CAM-01_Take_Photo_SD) | ESP32-CAM chụp ảnh lưu thẻ SD |
| [ESP32-CAM-02_MJPEG_Stream](ESP32-CAM-02_MJPEG_Stream) | ESP32-CAM stream video MJPEG |

### Dự án
| Project | Nội dung |
|---|---|
| [ESP32-PROJ-01_Weather_Station](ESP32-PROJ-01_Weather_Station) | Trạm thời tiết |
| [ESP32-PROJ-02_Neopixel_Light](ESP32-PROJ-02_Neopixel_Light) | Đèn LED NeoPixel |
| [ESP32-PROJ-03_Reaction_Game](ESP32-PROJ-03_Reaction_Game) | Game đo phản xạ |
| [ESP32-PROJ-04_GPS_Locator](ESP32-PROJ-04_GPS_Locator) | Định vị GPS |

## Cách sử dụng

1. Cài [VS Code](https://code.visualstudio.com/) và extension **PlatformIO IDE**.
2. Tải repo:
   ```bash
   git clone https://github.com/khuenguyencreator/esp32-arduino-examples.git
   ```
3. Trong VS Code chọn **PlatformIO → Open Project**, trỏ tới thư mục bài muốn chạy (thư mục có file `platformio.ini`).
4. Bấm **Build** rồi **Upload**, mở **Serial Monitor** để xem kết quả.
5. Với các bài dùng WiFi / Cloud, sửa SSID, mật khẩu và API key trong `src/main.cpp` trước khi nạp.

## Liên kết

- 📖 Bài viết hướng dẫn chi tiết: [khuenguyencreator.com](https://khuenguyencreator.com)
- 📚 Các repo khác: [github.com/khuenguyencreator](https://github.com/khuenguyencreator)

Nếu thấy hữu ích, hãy bấm ⭐ **Star** để ủng hộ nhé! Có lỗi hoặc thắc mắc, bạn tạo **Issue** trong repo này hoặc để lại bình luận trên blog.

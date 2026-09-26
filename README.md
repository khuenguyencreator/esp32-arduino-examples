# ESP32 Arduino Examples

Code ví dụ lập trình **ESP32** với **Arduino framework** trên **PlatformIO**, từ ngoại vi cơ bản (GPIO, ADC, PWM, Timer…) đến WiFi, Web Server, BLE, IoT và các dự án hoàn chỉnh. Code đi kèm series [Lập trình ESP32 từ A tới Z](https://khuenguyencreator.com/lap-trinh-esp32-tu-a-toi-z/) trên blog [khuenguyencreator.com](https://khuenguyencreator.com).

- Board: ESP32 DOIT DevKit V1 (`esp32doit-devkit-v1`); riêng nhóm `09-ESP32-CAM` dùng board **ESP32-CAM** (`esp32cam`)
- Framework: Arduino (`platform = espressif32`)
- IDE: VS Code + PlatformIO
- Serial Monitor: 115200 baud

## Cấu trúc repo

Mỗi thư mục nhóm được đánh số theo **thứ tự nên học**, bên trong mỗi bài là một project PlatformIO độc lập:

```
esp32-arduino-examples/
├── 01-Basic/             Ngoại vi cơ bản
├── 02-Sensor-I2C/        Cảm biến & màn hình I2C
├── 03-WebServer/         Web Server
├── 04-Protocol/          Giao thức truyền thông
├── 05-Bluetooth/         Bluetooth
├── 06-IoT/               IoT & Cloud
├── 07-OTA-WiFiConfig/    Cấu hình WiFi & nạp firmware từ xa
├── 08-FreeRTOS/          FreeRTOS
├── 09-ESP32-CAM/         ESP32-CAM
└── 10-Projects/          Dự án
```

## Danh sách bài

### [01-Basic](01-Basic) – Ngoại vi cơ bản

GPIO, ADC, PWM, DAC, ngắt, Timer, Touch và Deep Sleep. Nên học trước tiên.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_GPIO](01-Basic/01_GPIO) | GPIO Input / Output | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-gpio-digital-input-va-digital-output/) |
| [02_ADC](01-Basic/02_ADC) | Đọc ADC | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-analog-input-doc-tin-hieu-tuong-tu-adc/) |
| [03_PWM](01-Basic/03_PWM) | Băm xung PWM (LEDC) |  |
| [04_DAC](01-Basic/04_DAC) | Xuất DAC tạo xung hình sin | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-dac-tao-xung-hinh-sin/) |
| [05_External_Interrupt](01-Basic/05_External_Interrupt) | Ngắt ngoài |  |
| [06_1_Timer_Millis](01-Basic/06_1_Timer_Millis) | Định thời bằng `millis()` |  |
| [06_2_Timer_Interrupt](01-Basic/06_2_Timer_Interrupt) | Ngắt Timer phần cứng |  |
| [07_Touch_Pin](01-Basic/07_Touch_Pin) | Cảm biến chạm (Touch Pin) |  |
| [08_1_DeepSleep_Timer_WakeUp](01-Basic/08_1_DeepSleep_Timer_WakeUp) | Deep Sleep – đánh thức bằng Timer |  |
| [08_2_DeepSleep_Touch_WakeUp](01-Basic/08_2_DeepSleep_Touch_WakeUp) | Deep Sleep – đánh thức bằng Touch |  |
| [08_3_DeepSleep_EXT_WakeUp](01-Basic/08_3_DeepSleep_EXT_WakeUp) | Deep Sleep – đánh thức bằng chân ngoài |  |

### [02-Sensor-I2C](02-Sensor-I2C) – Cảm biến & màn hình I2C

Quét bus I2C, màn hình OLED và các cảm biến I2C thông dụng.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_I2C_Scan](02-Sensor-I2C/01_I2C_Scan) | Quét địa chỉ thiết bị I2C |  |
| [02_OLED_SH1106](02-Sensor-I2C/02_OLED_SH1106) | Màn hình OLED SH1106 |  |
| [03_SSD1306_BME280](02-Sensor-I2C/03_SSD1306_BME280) | Đọc cảm biến BME280, hiển thị lên OLED SSD1306 |  |
| [04_MPU6050](02-Sensor-I2C/04_MPU6050) | Cảm biến gia tốc / con quay MPU6050 |  |
| [05_APDS9960](02-Sensor-I2C/05_APDS9960) | Cảm biến cử chỉ / màu APDS9960 |  |

### [03-WebServer](03-WebServer) – Web Server

ESP32 làm Web Server: Station, Access Point, WebSocket, SPIFFS và giao tiếp 2 board.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_Station](03-WebServer/01_Station) | Web Server ở chế độ Station | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-webserver-che-do-wifi-station/) |
| [02_Access_Point](03-WebServer/02_Access_Point) | Web Server ở chế độ Access Point |  |
| [03_WebSocket](03-WebServer/03_WebSocket) | WebSocket điều khiển đèn real-time | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-websocket-dieu-khien-den-real-time/) |
| [04_SPIFFS_Webserver](03-WebServer/04_SPIFFS_Webserver) | Web Server lưu giao diện trên SPIFFS |  |
| [05_SPIFFS_Data_Storage](03-WebServer/05_SPIFFS_Data_Storage) | Lưu dữ liệu bền vững vào SPIFFS |  |
| [06_Two_Boards](03-WebServer/06_Two_Boards) | Giao tiếp 2 board ESP32 qua HTTP (`BoardA_Server` + `BoardB_Client`) |  |

### [04-Protocol](04-Protocol) – Giao thức truyền thông

HTTP, MQTT, HTTPS, TCP, UDP, WiFi Mesh và ESP-NOW.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_HTTP_OpenWeather](04-Protocol/01_HTTP_OpenWeather) | HTTP GET – lấy thời tiết từ OpenWeather |  |
| [02_HTTP_ThingSpeak](04-Protocol/02_HTTP_ThingSpeak) | HTTP – gửi dữ liệu DHT lên ThingSpeak |  |
| [03_MQTT](04-Protocol/03_MQTT) | MQTT bật tắt đèn với HiveMQ Broker | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-mqtt-bat-tat-den-voi-hivemq-broker/) |
| [04_HTTPS](04-Protocol/04_HTTPS) | Gọi API qua HTTPS (TLS) |  |
| [05_TCP_Socket](04-Protocol/05_TCP_Socket) | TCP Socket server |  |
| [06_UDP](04-Protocol/06_UDP) | UDP |  |
| [07_WiFi_Mesh](04-Protocol/07_WiFi_Mesh) | WiFi Mesh (painlessMesh) |  |
| [08_ESPNOW](04-Protocol/08_ESPNOW) | ESP-NOW, không cần router (`Sender` + `Receiver`) |  |

### [05-Bluetooth](05-Bluetooth) – Bluetooth

Bluetooth Classic và BLE. Chạy bài BLE Server trước rồi mới tới BLE Client.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_Classic_SPP](05-Bluetooth/01_Classic_SPP) | Bluetooth Classic (Serial Port Profile) |  |
| [02_BLE_Server](05-Bluetooth/02_BLE_Server) | BLE Server (GATT) |  |
| [03_BLE_Client](05-Bluetooth/03_BLE_Client) | BLE Client kết nối tới BLE Server |  |

### [06-IoT](06-IoT) – IoT & Cloud

Kết nối ESP32 với các dịch vụ Cloud.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_Firebase](06-IoT/01_Firebase) | Firebase Realtime Database |  |
| [02_NodeRED](06-IoT/02_NodeRED) | Node-RED |  |
| [03_Google_Sheets](06-IoT/03_Google_Sheets) | Ghi dữ liệu lên Google Sheets |  |
| [04_Send_Email](06-IoT/04_Send_Email) | Gửi Email qua SMTP |  |
| [05_Telegram_Bot](06-IoT/05_Telegram_Bot) | Điều khiển qua Telegram Bot |  |

### [07-OTA-WiFiConfig](07-OTA-WiFiConfig) – Cấu hình WiFi & nạp firmware từ xa

Không hardcode WiFi trong code, và cập nhật firmware không cần mạch nạp.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_SmartConfig](07-OTA-WiFiConfig/01_SmartConfig) | Cấu hình WiFi qua app bằng SmartConfig | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-smartconfig-thiet-lap-wifi/) |
| [02_WiFiManager_NTP](07-OTA-WiFiConfig/02_WiFiManager_NTP) | WiFiManager (captive portal) + lấy giờ NTP |  |
| [03_OTA_LAN_WebBrowser](07-OTA-WiFiConfig/03_OTA_LAN_WebBrowser) | Nạp firmware qua WiFi nội bộ bằng trình duyệt (OTA) | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-fota-nap-firmware-tu-xa-bang-local-wifi/) |
| [04_FOTA_Internet_OTAdrive](07-OTA-WiFiConfig/04_FOTA_Internet_OTAdrive) | Cập nhật firmware qua Internet với OTAdrive (FOTA) | [📖 Đọc](https://khuenguyencreator.com/lap-trinh-esp32-fota-nap-firmware-qua-mang-internet-voi-ota-drive/) |

### [08-FreeRTOS](08-FreeRTOS) – FreeRTOS

Chạy nhiều task song song trên 2 nhân của ESP32.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_FreeRTOS_Multitask](08-FreeRTOS/01_FreeRTOS_Multitask) | Tạo task FreeRTOS chạy song song |  |

### [09-ESP32-CAM](09-ESP32-CAM) – ESP32-CAM

Dùng board **ESP32-CAM (AI-Thinker)**, `board = esp32cam`, không phải DevKit V1.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_Take_Photo_SD](09-ESP32-CAM/01_Take_Photo_SD) | Chụp ảnh lưu thẻ SD |  |
| [02_MJPEG_Stream](09-ESP32-CAM/02_MJPEG_Stream) | Stream video MJPEG lên trình duyệt |  |

### [10-Projects](10-Projects) – Dự án

Các dự án hoàn chỉnh kết hợp kiến thức từ các nhóm trước.

| Bài | Nội dung | Bài viết |
|---|---|---|
| [01_Weather_Station](10-Projects/01_Weather_Station) | Trạm thời tiết (OpenWeather + OLED) |  |
| [02_Neopixel_Light](10-Projects/02_Neopixel_Light) | Đèn LED NeoPixel nhiều hiệu ứng |  |
| [03_Reaction_Game](10-Projects/03_Reaction_Game) | Game đo phản xạ |  |
| [04_GPS_Locator](10-Projects/04_GPS_Locator) | Định vị GPS NEO-6M |  |

## Cách sử dụng

1. Cài [VS Code](https://code.visualstudio.com/) và extension **PlatformIO IDE**.
2. Tải repo:
   ```bash
   git clone https://github.com/khuenguyencreator/esp32-arduino-examples.git
   ```
3. Trong VS Code chọn **PlatformIO → Open Project**, trỏ tới thư mục bài muốn chạy, ví dụ `01-Basic/01_GPIO` (thư mục có file `platformio.ini`). Với bài dùng 2 board (`03-WebServer/06_Two_Boards`, `04-Protocol/08_ESPNOW`), mở và nạp từng project con cho từng board.
4. Bấm **Build** rồi **Upload**, mở **Serial Monitor** để xem kết quả.
5. Với các bài dùng WiFi / Cloud, sửa SSID, mật khẩu và API key trong `src/main.cpp` trước khi nạp.

## Liên kết

- 📖 Bài viết hướng dẫn chi tiết: [khuenguyencreator.com](https://khuenguyencreator.com)
- 📚 Các repo khác: [github.com/khuenguyencreator](https://github.com/khuenguyencreator)

Nếu thấy hữu ích, hãy bấm ⭐ **Star** để ủng hộ nhé! Có lỗi hoặc thắc mắc, bạn tạo **Issue** trong repo này hoặc để lại bình luận trên blog.

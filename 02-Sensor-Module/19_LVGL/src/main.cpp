#include <Arduino.h>

#include <lvgl.h>
#include <TFT_eSPI.h>

// LVGL la thu vien giao dien do hoa dung nhieu nhat cho vi dieu khien: co san
// widget (nut, thanh truot, dong ho do, bieu do...), hieu ung, font chu dep -
// thay vi tu ve tung pixel nhu bai 18_TFT_eSPI.
//
// Kien truc: LVGL chi lo "ve cai gi", con viec day pixel ra man hinh van do
// TFT_eSPI lam qua ham my_disp_flush() ben duoi. Doi sang man hinh khac chi can
// sua ham nay, code giao dien giu nguyen.
//
// Bai nay hien thi gia tri bien tro (GPIO34) bang dong ho vong cung (arc).
// Noi man hinh giong bai 18_TFT_eSPI (thu vien LVGL 8.3).

const int POT_PIN = 34;

static const uint16_t SCREEN_WIDTH = 320;   // man ILI9341 xoay ngang
static const uint16_t SCREEN_HEIGHT = 240;

TFT_eSPI tft = TFT_eSPI();

// Bo dem ve: LVGL ve tung phan 20 dong mot, khong can du RAM cho ca man hinh
static lv_disp_draw_buf_t drawBuf;
static lv_color_t buf[SCREEN_WIDTH * 20];

lv_obj_t *arc;
lv_obj_t *valueLabel;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);   // true = dao byte cho dung thu tu SPI
  tft.endWrite();

  lv_disp_flush_ready(disp);   // bao LVGL da ve xong, duoc dung lai bo dem
}

void createUI() {
  lv_obj_t *title = lv_label_create(lv_scr_act());
  lv_label_set_text(title, "ESP32 + LVGL");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

  arc = lv_arc_create(lv_scr_act());
  lv_obj_set_size(arc, 180, 180);
  lv_arc_set_rotation(arc, 135);
  lv_arc_set_bg_angles(arc, 0, 270);
  lv_arc_set_range(arc, 0, 100);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);      // bo num keo, chi de hien thi
  lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_align(arc, LV_ALIGN_CENTER, 0, 15);

  valueLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(valueLabel, &lv_font_montserrat_28, 0);
  lv_obj_align(valueLabel, LV_ALIGN_CENTER, 0, 15);
}

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);

  lv_init();
  lv_disp_draw_buf_init(&drawBuf, buf, NULL, SCREEN_WIDTH * 20);

  static lv_disp_drv_t dispDrv;
  lv_disp_drv_init(&dispDrv);
  dispDrv.hor_res = SCREEN_WIDTH;
  dispDrv.ver_res = SCREEN_HEIGHT;
  dispDrv.flush_cb = my_disp_flush;
  dispDrv.draw_buf = &drawBuf;
  lv_disp_drv_register(&dispDrv);

  createUI();
}

void loop() {
  // LVGL can biet thoi gian troi qua de chay hieu ung - bao cho no bang millis()
  static uint32_t lastTick = millis();
  uint32_t now = millis();
  lv_tick_inc(now - lastTick);
  lastTick = now;

  static uint32_t lastUpdate = 0;
  if (now - lastUpdate > 200) {
    lastUpdate = now;
    int percent = map(analogRead(POT_PIN), 0, 4095, 0, 100);
    lv_arc_set_value(arc, percent);
    lv_label_set_text_fmt(valueLabel, "%d%%", percent);
  }

  lv_timer_handler();   // LVGL ve lai nhung vung vua thay doi
  delay(5);
}

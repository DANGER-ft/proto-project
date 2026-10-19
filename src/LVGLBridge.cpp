#include "LVGLBridge.h"
#include <lvgl.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;


static lv_color_t *lv_buf = nullptr;
static lv_disp_draw_buf_t lv_draw_buf;
static lv_disp_drv_t lv_disp_drv;

static void lv_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)color_p, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(drv);
}


void lvgl_bridge_init() {
    lv_init();

    size_t buf_size = TFT_WIDTH * TFT_HEIGHT * sizeof(lv_color_t);
    lv_buf = (lv_color_t*)ps_malloc(buf_size);
    if (!lv_buf) {
        Serial.println("[LVGL] PSRAM alloc failed, fallback to internal RAM");
        lv_buf = (lv_color_t*)malloc(buf_size);
    }
    if (!lv_buf) {
        Serial.println("[LVGL] FATAL: no memory for buffer");
        return;
    }
    Serial.printf("[LVGL] buffer: %u bytes at %p\n", (unsigned)buf_size, lv_buf);

    lv_disp_draw_buf_init(&lv_draw_buf, lv_buf, NULL, TFT_WIDTH * TFT_HEIGHT);

    lv_disp_drv_init(&lv_disp_drv);
    lv_disp_drv.hor_res = TFT_WIDTH;
    lv_disp_drv.ver_res = TFT_HEIGHT;
    lv_disp_drv.flush_cb = lv_flush_cb;
    lv_disp_drv.draw_buf = &lv_draw_buf;
    lv_disp_drv_register(&lv_disp_drv);

    Serial.println("[LVGL] bridge initialized");
}

void lvgl_bridge_tick() {
    lv_timer_handler();
}

#pragma once

// Инициализация LVGL (вызывать один раз в setup после tft.init())
void lvgl_bridge_init();

// Тик LVGL (вызывать регулярно в loop)
void lvgl_bridge_tick();

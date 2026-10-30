#include <WiFi.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <LittleFS.h>
#include <LuaWrapper.h>
#include <FS.h>
#include "SPI.h"
#include <esp_task_wdt.h>
#include "USBKeyboardHost.h"
#include "USBKeyboardLua.h"
#include <esp_system.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <esp_wifi.h>
#include <esp_mac.h>
#include "LVGLBridge.h"
#include "LuaLVGL.h"
#include "LVGLFs.h"
#include <lvgl.h>
using namespace fs;
#include "SimpleFTPServer.h" 

LuaWrapper lua;
TFT_eSPI tft = TFT_eSPI();
FtpServer ftpSrv;
#include <luaTFT.h>
#include <LuaUtilites.h>
#include <memory.h>

#include "configs.h" //проверьте чтобы был
static bool tftJpgOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}


void setup()
{
  Serial.begin(115200);
  usb_kb_init();
  esp_task_wdt_delete(NULL);
  
  if (!LittleFS.begin())
  {
    Serial.println("LittleFS error");
    return;
  }

  tft.init();
  tft.setSwapBytes(true);
  Serial.printf("PSRAM: %d / %d\n", ESP.getFreePsram(), ESP.getPsramSize());
  Serial.printf("Heap:  %d\n", ESP.getFreeHeap());
  lvgl_bridge_init();
  lvgl_fs_init();
  LuaCounter(lua);
  LuaTimer(lua);
  makeDelay(lua);
  makeMemory(lua);
  LuaKb(lua);
  LuaLVGL(lua);
  LuaTFT(lua);
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(false);
  TJpgDec.setCallback(tftJpgOutput);
  
  if (LittleFS.exists("/run.lua"))
  {
    File file = LittleFS.open("/run.lua", "r");
    String scriptRun = file.readString();
    Serial.println(lua.Lua_dostring(&scriptRun));
  }
  else
  {
  }
}

static uint32_t last = 0;
void loop()
{
  if (millis() - last >= 5)
  {
    last = millis();
    lvgl_bridge_tick();
    lv_tick_inc(5);
  }
}

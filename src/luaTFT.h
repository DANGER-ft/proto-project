#pragma once
#include <FS.h>
#include <LittleFS.h>
#include <PNGdec.h>
#undef INTELSHORT
#undef INTELLONG
#include <AnimatedGIF.h>
#include <lvgl.h>
#include <JPEGDEC.h>
#include "TFT_eSPI.h"
#include <TJpg_Decoder.h>
extern TFT_eSPI tft;
static uint8_t hexNibble(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return 0;
}

static uint16_t hexTo565(const char *hex)
{
    if (!hex)
        return 0;
    if (*hex == '#')
        hex++;

    uint32_t r = 0, g = 0, b = 0;
    size_t len = strlen(hex);

    if (len == 3)
    {
        r = hexNibble(hex[0]) * 17;
        g = hexNibble(hex[1]) * 17;
        b = hexNibble(hex[2]) * 17;
    }
    else if (len >= 6)
    {
        r = (hexNibble(hex[0]) << 4) | hexNibble(hex[1]);
        g = (hexNibble(hex[2]) << 4) | hexNibble(hex[3]);
        b = (hexNibble(hex[4]) << 4) | hexNibble(hex[5]);
    }
    else
        return 0;

    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

static uint16_t rgbTo565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
static int lua_drawPixel(lua_State *L)
{
    tft.drawPixel(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}
static int lua_drawLine(lua_State *L)
{
    tft.drawLine(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5));
    return 0;
}
static int lua_drawFastVLine(lua_State *L)
{
    tft.drawFastVLine(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
    return 0;
}
static int lua_drawFastHLine(lua_State *L)
{
    tft.drawFastHLine(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
    return 0;
}
static int lua_drawRect(lua_State *L)
{
    tft.drawRect(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5));
    return 0;
}
static int lua_fillRect(lua_State *L)
{
    tft.fillRect(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5));
    return 0;
}
static int lua_drawRoundRect(lua_State *L)
{
    tft.drawRoundRect(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6));
    return 0;
}
static int lua_fillRoundRect(lua_State *L)
{
    tft.fillRoundRect(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6));
    return 0;
}

static int lua_drawCircle(lua_State *L)
{
    tft.drawCircle(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
    return 0;
}
static int lua_fillCircle(lua_State *L)
{
    tft.fillCircle(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
    return 0;
}
static int lua_drawTriangle(lua_State *L)
{
    tft.drawTriangle(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6), luaL_checkinteger(L, 7));
    return 0;
}
static int lua_fillTriangle(lua_State *L)
{
    tft.fillTriangle(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6), luaL_checkinteger(L, 7));
    return 0;
}

static int lua_fillRectHGradient(lua_State *L)
{
    tft.fillRectHGradient(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6));
    return 0;
}
static int lua_fillRectVGradient(lua_State *L)
{
    tft.fillRectVGradient(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6));
    return 0;
}
static int lua_fillScreen(lua_State *L)
{
    tft.fillScreen(luaL_checkinteger(L, 1));
    return 0;
}

static int lua_setTextColor(lua_State *L)
{

    tft.setTextColor(luaL_checkinteger(L, 1));

    return 0;
}
static int lua_setTextSize(lua_State *L)
{
    tft.setTextSize(luaL_checkinteger(L, 1));
    return 0;
}
static int lua_drawString(lua_State *L)
{
    tft.drawString(luaL_checkstring(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}
static int lua_drawCentreString(lua_State *L)
{
    tft.drawCentreString(luaL_checkstring(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), 1);
    return 0;
}
static int lua_setRotation(lua_State *L)
{
    tft.setRotation(luaL_checkinteger(L, 1));
    return 0;
}
static int lua_getRotation(lua_State *L)
{
    lua_pushinteger(L, tft.getRotation());
    return 1;
}
static int lua_width(lua_State *L)
{
    lua_pushinteger(L, tft.width());
    return 1;
}
static int lua_height(lua_State *L)
{
    lua_pushinteger(L, tft.height());
    return 1;
}
static int lua_invertDisplay(lua_State *L)
{
    tft.invertDisplay(lua_toboolean(L, 1));
    return 0;
}
static int lua_pushImage(lua_State *L)
{
    int16_t x = luaL_checkinteger(L, 1);
    int16_t y = luaL_checkinteger(L, 2);
    int16_t w = luaL_checkinteger(L, 3);
    int16_t h = luaL_checkinteger(L, 4);
    uint16_t *data = (uint16_t *)lua_touserdata(L, 5);
    bool swap = lua_toboolean(L, 6);
    tft.pushImage(x, y, w, h, data, swap);
    return 0;
}



static lv_obj_t *lvMakeCanvas(lv_color_t *buf, int w, int h, int x, int y)
{
    lv_obj_t *c = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(c, buf, w, h, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_pos(c, x, y);
    return c;
}
static void tftMakeCanvas(uint16_t *buf, int w, int h, int x, int y)
{
    tft.pushImage(x, y, w, h, buf);
}

static JPEGDEC jpg;
static lv_color_t *jpg_buf = nullptr;
static uint16_t *jpg_buf16 = nullptr;
static int jpg_w = 0, jpg_h = 0;

static int jpgDraw(JPEGDRAW *pDraw)
{
    uint16_t *src = pDraw->pPixels;
    for (int r = 0; r < pDraw->iHeight; r++)
    {
        uint16_t *dst = (uint16_t *)jpg_buf + (pDraw->y + r) * jpg_w + pDraw->x;
        memcpy(dst, src + r * pDraw->iWidth, pDraw->iWidth * 2);
    }
    return 1;
}

static bool renderJpg(const String &path, int x, int y)
{
    File f = LittleFS.open(path, "r");
    if (!f)
        return false;
    if (!jpg.open(f, jpgDraw))
    {
        f.close();
        return false;
    }

    jpg_w = jpg.getWidth();
    jpg_h = jpg.getHeight();
    if (jpg_w <= 0 || jpg_h <= 0)
    {
        jpg.close();
        f.close();
        return false;
    }

    jpg_buf = (lv_color_t *)malloc(jpg_w * jpg_h * sizeof(lv_color_t));
    if (!jpg_buf)
    {
        jpg.close();
        f.close();
        return false;
    }
    memset(jpg_buf, 0, jpg_w * jpg_h * sizeof(lv_color_t));

    jpg.setPixelType(RGB565_LITTLE_ENDIAN);
    int ok = jpg.decode(0, 0, 0);
    jpg.close();
    f.close();

    if (!ok)
    {
        free(jpg_buf);
        jpg_buf = nullptr;
        return false;
    }
    lvMakeCanvas(jpg_buf, jpg_w, jpg_h, x, y);
    return true;
}

static bool tftRenderJpg(const String &path, int x, int y)
{
    TJpgDec.drawFsJpg(x, y, path.c_str(), LittleFS);
    return true;
}

static uint16_t read16(File &f)
{
    uint16_t r;
    ((uint8_t *)&r)[0] = f.read();
    ((uint8_t *)&r)[1] = f.read();
    return r;
}
static uint32_t read32(File &f)
{
    uint32_t r;
    ((uint8_t *)&r)[0] = f.read();
    ((uint8_t *)&r)[1] = f.read();
    ((uint8_t *)&r)[2] = f.read();
    ((uint8_t *)&r)[3] = f.read();
    return r;
}

static bool renderBmp(const String &path, int x, int y)
{
    File f = LittleFS.open(path, "r");
    if (!f)
        return false;
    if (read16(f) != 0x4D42)
    {
        f.close();
        return false;
    }
    read32(f);
    read32(f);
    uint32_t off = read32(f);
    read32(f);
    uint16_t w = read32(f);
    uint16_t h = read32(f);
    if (read16(f) != 1 || read16(f) != 24 || read32(f) != 0)
    {
        f.close();
        return false;
    }

    uint16_t *buf = (uint16_t *)malloc(w * h * 2);
    if (!buf)
    {
        f.close();
        return false;
    }

    f.seek(off);
    uint16_t pad = (4 - ((w * 3) & 3)) & 3;
    uint8_t *row = (uint8_t *)malloc(w * 3 + pad);
    if (!row)
    {
        free(buf);
        f.close();
        return false;
    }

    for (int r = 0; r < h; r++)
    {
        f.read(row, w * 3 + pad);
        uint8_t *bp = row;
        uint16_t *dst = buf + (h - 1 - r) * w;
        for (int c = 0; c < w; c++)
        {
            uint8_t b = *bp++, g = *bp++, rr = *bp++;
            dst[c] = rgbTo565(rr, g, b);
        }
    }
    free(row);
    f.close();

    lvMakeCanvas((lv_color_t *)buf, w, h, x, y);
    return true;
}

static bool tftRenderBmp(const String &path, int x, int y)
{
    File f = LittleFS.open(path, "r");
    if (!f)
        return false;
    if (read16(f) != 0x4D42)
    {
        f.close();
        return false;
    }
    read32(f);
    read32(f);
    uint32_t off = read32(f);
    read32(f);
    uint16_t w = read32(f);
    uint16_t h = read32(f);
    if (read16(f) != 1 || read16(f) != 24 || read32(f) != 0)
    {
        f.close();
        return false;
    }

    uint16_t *buf = (uint16_t *)malloc(w * h * 2);
    if (!buf)
    {
        f.close();
        return false;
    }

    f.seek(off);
    uint16_t pad = (4 - ((w * 3) & 3)) & 3;
    uint8_t *row = (uint8_t *)malloc(w * 3 + pad);
    if (!row)
    {
        free(buf);
        f.close();
        return false;
    }

    for (int r = 0; r < h; r++)
    {
        f.read(row, w * 3 + pad);
        uint8_t *bp = row;
        uint16_t *dst = buf + (h - 1 - r) * w;
        for (int c = 0; c < w; c++)
        {
            uint8_t b = *bp++, g = *bp++, rr = *bp++;
            dst[c] = rgbTo565(rr, g, b);
        }
    }
    free(row);
    f.close();

    tftMakeCanvas(buf, w, h, x, y);
    free(buf);
    return true;
}

static File pngFile;

static void *pngOpen(const char *fn, int32_t *sz)
{
    pngFile = LittleFS.open(fn);
    *sz = pngFile.size();
    return &pngFile;
}
static void pngClose(void *)
{
    if (pngFile)
        pngFile.close();
}
static int32_t pngRead(PNGFILE *, uint8_t *b, int32_t l) { return pngFile ? pngFile.read(b, l) : 0; }
static int32_t pngSeek(PNGFILE *, int32_t p) { return pngFile ? pngFile.seek(p) : 0; }

static PNG *png = nullptr;
static lv_color_t *png_buf = nullptr;
static uint16_t *png_buf16 = nullptr;
static int png_w = 0;

static int pngDraw(PNGDRAW *p)
{
    static uint16_t line[320];
    png->getLineAsRGB565(p, line, PNG_RGB565_LITTLE_ENDIAN, 0);
    memcpy((uint16_t *)png_buf + p->y * png_w, line, p->iWidth * 2);
    return 1;
}

static bool renderPng(const String &path, int x, int y)
{
    png = new PNG();
    if (png->open(path.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw) != PNG_SUCCESS)
    {
        delete png;
        png = nullptr;
        return false;
    }

    png_w = png->getWidth();
    int png_h = png->getHeight();
    png_buf = (lv_color_t *)malloc(png_w * png_h * sizeof(lv_color_t));
    if (!png_buf)
    {
        png->close();
        delete png;
        png = nullptr;
        return false;
    }

    int rc = png->decode(NULL, 0);
    png->close();
    delete png;
    png = nullptr;

    if (rc != PNG_SUCCESS)
    {
        free(png_buf);
        png_buf = nullptr;
        return false;
    }
    lvMakeCanvas(png_buf, png_w, png_h, x, y);
    return true;
}

static int tftPngDraw(PNGDRAW *p)
{
    static uint16_t line[320];
    png->getLineAsRGB565(p, line, PNG_RGB565_LITTLE_ENDIAN, 0);
    memcpy(png_buf16 + p->y * png_w, line, p->iWidth * 2);
    return 1;
}

static bool tftRenderPng(const String &path, int x, int y)
{
    png = new PNG();
    if (png->open(path.c_str(), pngOpen, pngClose, pngRead, pngSeek, tftPngDraw) != PNG_SUCCESS)
    {
        delete png;
        png = nullptr;
        return false;
    }

    png_w = png->getWidth();
    int png_h = png->getHeight();
    png_buf16 = (uint16_t *)malloc(png_w * png_h * 2);
    if (!png_buf16)
    {
        png->close();
        delete png;
        png = nullptr;
        return false;
    }

    int rc = png->decode(NULL, 0);
    png->close();
    delete png;
    png = nullptr;

    if (rc != PNG_SUCCESS)
    {
        free(png_buf16);
        png_buf16 = nullptr;
        return false;
    }
    tftMakeCanvas(png_buf16, png_w, png_h, x, y);
    free(png_buf16);
    png_buf16 = nullptr;
    return true;
}

static AnimatedGIF *gif = nullptr;

static void *gifOpen(const char *fn, int32_t *sz)
{
    File *f = new File(LittleFS.open(fn));
    if (!f || !*f)
    {
        delete f;
        return nullptr;
    }
    *sz = f->size();
    return f;
}
static void gifClose(void *h)
{
    File *f = (File *)h;
    if (f)
    {
        f->close();
        delete f;
    }
}
static int32_t gifRead(GIFFILE *p, uint8_t *b, int32_t l)
{
    File *f = (File *)p->fHandle;
    if ((p->iSize - p->iPos) < l)
        l = p->iSize - p->iPos;
    if (l <= 0)
        return 0;
    int32_t n = f->read(b, l);
    p->iPos = f->position();
    return n;
}
static int32_t gifSeek(GIFFILE *p, int32_t pos)
{
    File *f = (File *)p->fHandle;
    f->seek(pos);
    p->iPos = f->position();
    return p->iPos;
}

static int16_t gifX = 0, gifY = 0;
static void gifDraw(GIFDRAW *p)
{
    static uint16_t line[320];
    uint8_t *s = p->pPixels;
    uint16_t *pal = p->pPalette;
    int w = p->iWidth;
    int y = p->iY + p->y;

    if (p->ucHasTransparency)
    {
        int x = 0;
        while (x < w)
        {
            int n = 0;
            while (x + n < w && s[n] != p->ucTransparent)
                n++;
            if (n)
            {
                for (int i = 0; i < n; i++)
                    line[i] = pal[s[i]];
                tft.pushImage(gifX + p->iX + x, gifY + y, n, 1, line);
                x += n;
                s += n;
            }
            while (x < w && *s == p->ucTransparent)
            {
                s++;
                x++;
            }
        }
    }
    else
    {
        for (int i = 0; i < w; i++)
            line[i] = pal[s[i]];
        tft.pushImage(gifX + p->iX, gifY + y, w, 1, line);
    }
}

static bool renderGif(const String &path, int x, int y, int loops)
{
    if (!gif)
        gif = new AnimatedGIF();
    gif->begin(BIG_ENDIAN_PIXELS);
    if (!gif->open(path.c_str(), gifOpen, gifClose, gifRead, gifSeek, gifDraw))
        return false;

    gifX = x;
    gifY = y;
    int total = (loops < 0) ? -1 : loops + 1;
    int done = 0;
    while (total < 0 || done < total)
    {
        int r, delayMs = 0;
        while ((r = gif->playFrame(true, &delayMs)) > 0)
            if (delayMs < 50)
                delay(50 - delayMs);
        if (r < 0)
            break;
        done++;
    }
    gif->close();
    return true;
}


bool drawImage(const String &path, int x = 0, int y = 0, int loops = -1)
{
    if (!LittleFS.exists(path))
        return false;
    int dot = path.lastIndexOf('.');
    if (dot < 0)
        return false;
    String ext = path.substring(dot + 1);
    ext.toLowerCase();

    if (ext == "jpg" || ext == "jpeg")
        return renderJpg(path, x, y);
    if (ext == "bmp")
        return renderBmp(path, x, y);
    if (ext == "png")
        return renderPng(path, x, y);
    if (ext == "gif")
        return renderGif(path, x, y, loops);
    return false;
}

bool tftDrawImage(const String &path, int x = 0, int y = 0, int loops = -1)
{
    if (!LittleFS.exists(path))
        return false;
    int dot = path.lastIndexOf('.');
    if (dot < 0)
        return false;
    String ext = path.substring(dot + 1);
    ext.toLowerCase();

    if (ext == "jpg" || ext == "jpeg")
        return tftRenderJpg(path, x, y);
    if (ext == "bmp")
        return tftRenderBmp(path, x, y);
    if (ext == "png")
        return tftRenderPng(path, x, y);
    if (ext == "gif")
        return renderGif(path, x, y, loops);
    return false;
}

static int drawImg(lua_State *lua_state)
{
    drawImage(luaL_checkstring(lua_state, 1),
              luaL_checknumber(lua_state, 2),
              luaL_checknumber(lua_state, 3),
              luaL_checknumber(lua_state, 4));
    return 0;
}

static int drawfsImg(lua_State *lua_state)
{
    tftDrawImage(luaL_checkstring(lua_state, 3),
                 luaL_checknumber(lua_state, 1),
                 luaL_checknumber(lua_state, 2),
                 0);
    return 0;
}
static int drawfsGif(lua_State *lua_state)
{
    tftDrawImage(luaL_checkstring(lua_state, 4),
                 luaL_checknumber(lua_state, 1),
                 luaL_checknumber(lua_state, 2),
                 luaL_checknumber(lua_state, 3));
    return 0;
}

static int lua_rgbTo565(lua_State *L)
{
    lua_pushinteger(L, rgbTo565(
                           luaL_checkinteger(L, 1),
                           luaL_checkinteger(L, 2),
                           luaL_checkinteger(L, 3)));
    return 1;
}
static int lua_hexTo565(lua_State *L)
{
    lua_pushinteger(L, hexTo565(luaL_checkstring(L, 1)));
    return 1;
}

#define PNG_CACHE_SIZE 3
#define PNG_MAX_WIDTH  128
#define PNG_MAX_HEIGHT 160

struct PngCacheEntry {
    String path;
    uint16_t *pixels;
    int width;
    int height;
    unsigned long lastUsed;
    bool used;
};

static PngCacheEntry pngCache[PNG_CACHE_SIZE];
static uint16_t *pngDecodeBuffer = nullptr;
static int pngDecodeWidth = 0;
static int pngDecodeHeight = 0;
static uint16_t pngLineBuffer[PNG_MAX_WIDTH];


static void* png_alloc(size_t size) {
    void *p = ps_malloc(size);
    if (!p) {
        Serial.println("[PNG] ps_malloc failed, fallback to malloc");
        p = malloc(size);
    }
    return p;
}


static int pngCacheDrawCallback(PNGDRAW *pDraw) {
    if (pDraw->y >= pngDecodeHeight) return 1;
    png->getLineAsRGB565(pDraw, pngLineBuffer, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
    int y = pDraw->y;
    int w = (pDraw->iWidth > pngDecodeWidth) ? pngDecodeWidth : pDraw->iWidth;
    memcpy(&pngDecodeBuffer[y * pngDecodeWidth], pngLineBuffer, w * sizeof(uint16_t));
    return 1;
}


static PngCacheEntry* pngCacheFind(const String &path) {
    for (int i = 0; i < PNG_CACHE_SIZE; i++) {
        if (pngCache[i].used && pngCache[i].path == path) {
            pngCache[i].lastUsed = millis();
            return &pngCache[i];
        }
    }
    return nullptr;
}


static int pngCacheFindSlot() {
    for (int i = 0; i < PNG_CACHE_SIZE; i++) {
        if (!pngCache[i].used) return i;
    }
    int oldest = 0;
    unsigned long oldestTime = pngCache[0].lastUsed;
    for (int i = 1; i < PNG_CACHE_SIZE; i++) {
        if (pngCache[i].lastUsed < oldestTime) {
            oldestTime = pngCache[i].lastUsed;
            oldest = i;
        }
    }
    if (pngCache[oldest].pixels) {
        free(pngCache[oldest].pixels);
        pngCache[oldest].pixels = nullptr;
    }
    pngCache[oldest].used = false;
    pngCache[oldest].path = "";
    Serial.printf("[PNG] evicted slot %d\n", oldest);
    return oldest;
}


static bool pngLoad(const String &path) {

    if (!png) png = new PNG();

    int rc = png->open(path.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngCacheDrawCallback);
    if (rc != PNG_SUCCESS) {
        Serial.printf("[PNG] open failed: %d, path=%s\n", rc, path.c_str());
        delete png;
        png = nullptr;
        return false;
    }

    pngDecodeWidth  = png->getWidth();
    pngDecodeHeight = png->getHeight();
    if (pngDecodeWidth  > PNG_MAX_WIDTH)  pngDecodeWidth  = PNG_MAX_WIDTH;
    if (pngDecodeHeight > PNG_MAX_HEIGHT) pngDecodeHeight = PNG_MAX_HEIGHT;

    size_t bytes = (size_t)pngDecodeWidth * pngDecodeHeight * sizeof(uint16_t);
    pngDecodeBuffer = (uint16_t*)png_alloc(bytes);
    if (!pngDecodeBuffer) {
        Serial.printf("[PNG] alloc failed: %u bytes\n", (unsigned)bytes);
        png->close();
        delete png;
        png = nullptr;
        return false;
    }

    rc = png->decode(nullptr, 0);
    png->close();
    delete png;
    png = nullptr;

    if (rc != PNG_SUCCESS) {
        Serial.printf("[PNG] decode failed: %d\n", rc);
        free(pngDecodeBuffer);
        pngDecodeBuffer = nullptr;
        return false;
    }

    int slot = pngCacheFindSlot();
    pngCache[slot].path     = path;
    pngCache[slot].pixels   = pngDecodeBuffer;
    pngCache[slot].width    = pngDecodeWidth;
    pngCache[slot].height   = pngDecodeHeight;
    pngCache[slot].lastUsed = millis();
    pngCache[slot].used     = true;

    pngDecodeBuffer = nullptr;
    Serial.printf("[PNG] cached '%s' -> slot %d (%dx%d)\n",
                  path.c_str(), slot, pngDecodeWidth, pngDecodeHeight);
    return true;
}


bool drawPngCached(const String &path, int x, int y) {
    PngCacheEntry *entry = pngCacheFind(path);
    if (!entry) {
        if (!pngLoad(path)) return false;
        entry = pngCacheFind(path);
        if (!entry) return false;
    }
    tft.pushImage(x, y, entry->width, entry->height, entry->pixels);
    return true;
}


void clearPngCache() {
    for (int i = 0; i < PNG_CACHE_SIZE; i++) {
        if (pngCache[i].pixels) {
            free(pngCache[i].pixels);
            pngCache[i].pixels = nullptr;
        }
        pngCache[i].used = false;
        pngCache[i].path = "";
    }
}



#define PNG_SMALL_CACHE_SIZE 32
#define PNG_SMALL_MAX        56

struct PngSmallCacheEntry {
    String path;
    uint16_t *pixels;
    int width;
    int height;
    unsigned long lastUsed;
    bool used;
};

static PngSmallCacheEntry pngSmallCache[PNG_SMALL_CACHE_SIZE];
static uint16_t *pngSmallDecodeBuffer = nullptr;
static int pngSmallDecodeWidth = 0;
static int pngSmallDecodeHeight = 0;
static uint16_t pngSmallLineBuffer[PNG_SMALL_MAX];


static int pngSmallDrawCallback(PNGDRAW *pDraw) {
    if (pDraw->y >= pngSmallDecodeHeight) return 1;
    png->getLineAsRGB565(pDraw, pngSmallLineBuffer, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
    int y = pDraw->y;
    int w = (pDraw->iWidth > pngSmallDecodeWidth) ? pngSmallDecodeWidth : pDraw->iWidth;
    memcpy(&pngSmallDecodeBuffer[y * pngSmallDecodeWidth], pngSmallLineBuffer, w * sizeof(uint16_t));
    return 1;
}


static PngSmallCacheEntry* pngSmallCacheFind(const String &path) {
    for (int i = 0; i < PNG_SMALL_CACHE_SIZE; i++) {
        if (pngSmallCache[i].used && pngSmallCache[i].path == path) {
            pngSmallCache[i].lastUsed = millis();
            return &pngSmallCache[i];
        }
    }
    return nullptr;
}


static int pngSmallCacheFindSlot() {
    for (int i = 0; i < PNG_SMALL_CACHE_SIZE; i++) {
        if (!pngSmallCache[i].used) return i;
    }
    int oldest = 0;
    unsigned long oldestTime = pngSmallCache[0].lastUsed;
    for (int i = 1; i < PNG_SMALL_CACHE_SIZE; i++) {
        if (pngSmallCache[i].lastUsed < oldestTime) {
            oldestTime = pngSmallCache[i].lastUsed;
            oldest = i;
        }
    }
    if (pngSmallCache[oldest].pixels) {
        free(pngSmallCache[oldest].pixels);
        pngSmallCache[oldest].pixels = nullptr;
    }
    pngSmallCache[oldest].used = false;
    pngSmallCache[oldest].path = "";
    Serial.printf("[PNG-S] evicted slot %d\n", oldest);
    return oldest;
}


static bool pngSmallLoad(const String &path) {
    if (!png) png = new PNG();

    int rc = png->open(path.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngSmallDrawCallback);
    if (rc != PNG_SUCCESS) {
        Serial.printf("[PNG-S] open failed: %d, path=%s\n", rc, path.c_str());
        delete png;
        png = nullptr;
        return false;
    }

    pngSmallDecodeWidth  = png->getWidth();
    pngSmallDecodeHeight = png->getHeight();

    if (pngSmallDecodeWidth  > PNG_SMALL_MAX)  pngSmallDecodeWidth  = PNG_SMALL_MAX;
    if (pngSmallDecodeHeight > PNG_SMALL_MAX) pngSmallDecodeHeight = PNG_SMALL_MAX;

    size_t bytes = (size_t)pngSmallDecodeWidth * pngSmallDecodeHeight * sizeof(uint16_t);
    pngSmallDecodeBuffer = (uint16_t*)png_alloc(bytes);
    if (!pngSmallDecodeBuffer) {
        Serial.printf("[PNG-S] alloc failed: %u bytes\n", (unsigned)bytes);
        png->close();
        delete png;
        png = nullptr;
        return false;
    }

    rc = png->decode(nullptr, 0);
    png->close();
    delete png;
    png = nullptr;

    if (rc != PNG_SUCCESS) {
        Serial.printf("[PNG-S] decode failed: %d\n", rc);
        free(pngSmallDecodeBuffer);
        pngSmallDecodeBuffer = nullptr;
        return false;
    }

    int slot = pngSmallCacheFindSlot();
    pngSmallCache[slot].path     = path;
    pngSmallCache[slot].pixels   = pngSmallDecodeBuffer;
    pngSmallCache[slot].width    = pngSmallDecodeWidth;
    pngSmallCache[slot].height   = pngSmallDecodeHeight;
    pngSmallCache[slot].lastUsed = millis();
    pngSmallCache[slot].used     = true;

    pngSmallDecodeBuffer = nullptr;
    Serial.printf("[PNG-S] cached '%s' -> slot %d (%dx%d)\n",
                  path.c_str(), slot, pngSmallDecodeWidth, pngSmallDecodeHeight);
    return true;
}


bool drawPngSmallCached(const String &path, int x, int y) {
    PngSmallCacheEntry *entry = pngSmallCacheFind(path);
    if (!entry) {
        if (!pngSmallLoad(path)) return false;
        entry = pngSmallCacheFind(path);
        if (!entry) return false;
    }
    tft.pushImage(x, y, entry->width, entry->height, entry->pixels);
    return true;
}


void clearPngSmallCache() {
    for (int i = 0; i < PNG_SMALL_CACHE_SIZE; i++) {
        if (pngSmallCache[i].pixels) {
            free(pngSmallCache[i].pixels);
            pngSmallCache[i].pixels = nullptr;
        }
        pngSmallCache[i].used = false;
        pngSmallCache[i].path = "";
    }
}


static int lua_drawPngSmallCached(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    lua_pushboolean(L, drawPngSmallCached(String(path), x, y));
    return 1;
}


bool drawPngSmart(const String &path, int x, int y) {

    PngCacheEntry *big = pngCacheFind(path);
    if (big) {
        tft.pushImage(x, y, big->width, big->height, big->pixels);
        return true;
    }


    PngSmallCacheEntry *small = pngSmallCacheFind(path);
    if (small) {
        tft.pushImage(x, y, small->width, small->height, small->pixels);
        return true;
    }


    if (!png) png = new PNG();
    int rc = png->open(path.c_str(), pngOpen, pngClose, pngRead, pngSeek, nullptr);
    if (rc != PNG_SUCCESS) {
        Serial.printf("[PNG] open failed (smart): %d, path=%s\n", rc, path.c_str());
        delete png;
        png = nullptr;
        return false;
    }
    int w = png->getWidth();
    int h = png->getHeight();
    png->close();
    delete png;
    png = nullptr;


    if (w <= PNG_SMALL_MAX && h <= PNG_SMALL_MAX) {
        if (!pngSmallLoad(path)) return false;
        small = pngSmallCacheFind(path);
        if (!small) return false;
        tft.pushImage(x, y, small->width, small->height, small->pixels);
        return true;
    } else {
        if (!pngLoad(path)) return false;
        big = pngCacheFind(path);
        if (!big) return false;
        tft.pushImage(x, y, big->width, big->height, big->pixels);
        return true;
    }
}


static int lua_drawPngSmart(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    lua_pushboolean(L, drawPngSmart(String(path), x, y));
    return 1;
}
void LuaTFT(LuaWrapper &lua)
{

    lua.Lua_register("fastPngDraw", (const lua_CFunction)&lua_drawPngSmart);
    lua.Lua_register("hexTo565", (const lua_CFunction)&lua_hexTo565);
    lua.Lua_register("rgbTo565", (const lua_CFunction)&lua_rgbTo565);
    lua.Lua_register("drawFsGif", (const lua_CFunction)&drawfsGif);
    lua.Lua_register("drawFsImage", (const lua_CFunction)&drawfsImg);
    lua.Lua_register("drawImage", (const lua_CFunction)&drawImg);
    lua.Lua_register("drawPixel", (const lua_CFunction)&lua_drawPixel);
    lua.Lua_register("drawLine", (const lua_CFunction)&lua_drawLine);
    lua.Lua_register("drawFastVLine", (const lua_CFunction)&lua_drawFastVLine);
    lua.Lua_register("drawFastHLine", (const lua_CFunction)&lua_drawFastHLine);
    lua.Lua_register("drawRect", (const lua_CFunction)&lua_drawRect);
    lua.Lua_register("fillRect", (const lua_CFunction)&lua_fillRect);
    lua.Lua_register("drawRoundRect", (const lua_CFunction)&lua_drawRoundRect);
    lua.Lua_register("fillRoundRect", (const lua_CFunction)&lua_fillRoundRect);
    lua.Lua_register("drawCircle", (const lua_CFunction)&lua_drawCircle);
    lua.Lua_register("fillCircle", (const lua_CFunction)&lua_fillCircle);
    lua.Lua_register("drawTriangle", (const lua_CFunction)&lua_drawTriangle);
    lua.Lua_register("fillTriangle", (const lua_CFunction)&lua_fillTriangle);
    lua.Lua_register("fillRectHGradient", (const lua_CFunction)&lua_fillRectHGradient);
    lua.Lua_register("fillRectVGradient", (const lua_CFunction)&lua_fillRectVGradient);
    lua.Lua_register("fillScreen", (const lua_CFunction)&lua_fillScreen);

    lua.Lua_register("setTextColor", (const lua_CFunction)&lua_setTextColor);
    lua.Lua_register("setTextSize", (const lua_CFunction)&lua_setTextSize);
    lua.Lua_register("drawString", (const lua_CFunction)&lua_drawString);
    lua.Lua_register("drawCentreString", (const lua_CFunction)&lua_drawCentreString);

    lua.Lua_register("setRotation", (const lua_CFunction)&lua_setRotation);
    lua.Lua_register("getRotation", (const lua_CFunction)&lua_getRotation);
    lua.Lua_register("width", (const lua_CFunction)&lua_width);
    lua.Lua_register("height", (const lua_CFunction)&lua_height);
    lua.Lua_register("invertDisplay", (const lua_CFunction)&lua_invertDisplay);
    lua.Lua_register("pushImage", (const lua_CFunction)&lua_pushImage);
}

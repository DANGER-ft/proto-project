#pragma once
#include <LuaWrapper.h>
#include <LittleFS.h>
#include <esp_system.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <esp_wifi.h>
#include <esp_mac.h>
extern FtpServer ftpSrv;
static int wifiSetMac(lua_State *L)
{
    const char *mac = luaL_checkstring(L, 1);
    unsigned int b[6];
    if (sscanf(mac, "%x:%x:%x:%x:%x:%x",
               &b[0], &b[1], &b[2], &b[3], &b[4], &b[5]) != 6)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    uint8_t newMac[6];
    for (int i = 0; i < 6; i++)
        newMac[i] = (uint8_t)b[i];
    if (newMac[0] & 0x01)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    bool allZero = true;
    for (int i = 0; i < 6; i++)
        if (newMac[i])
        {
            allZero = false;
            break;
        }
    if (allZero)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    wifi_mode_t mode;
    if (esp_wifi_get_mode(&mode) != ESP_OK)
    {
        WiFi.mode(WIFI_STA);
    }
    esp_err_t err = esp_wifi_set_mac(WIFI_IF_STA, newMac);
    lua_pushboolean(L, err == ESP_OK);
    return 1;
}
static int wifiSetApMac(lua_State *L)
{
    const char *mac = luaL_checkstring(L, 1);
    unsigned int b[6];
    if (sscanf(mac, "%x:%x:%x:%x:%x:%x",
               &b[0], &b[1], &b[2], &b[3], &b[4], &b[5]) != 6)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    uint8_t newMac[6];
    for (int i = 0; i < 6; i++)
        newMac[i] = (uint8_t)b[i];
    if (newMac[0] & 0x01)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    wifi_mode_t mode;
    if (esp_wifi_get_mode(&mode) != ESP_OK)
    {
        WiFi.mode(WIFI_AP);
    }
    esp_err_t err = esp_wifi_set_mac(WIFI_IF_AP, newMac);
    lua_pushboolean(L, err == ESP_OK);
    return 1;
}
static int wifiGetMac(lua_State *L)
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    lua_pushstring(L, buf);
    return 1;
}
static int wifiScan(lua_State *L)
{
    int n = WiFi.scanNetworks();
    lua_newtable(L);
    if (n <= 0)
    {
        return 1;
    }
    int index = 1;
    for (int i = 0; i < n; i++)
    {
        String entry = WiFi.SSID(i) + "|" +
                       String(WiFi.RSSI(i)) + "|" +
                       String(WiFi.channel(i)) + "|" +
                       String((int)WiFi.encryptionType(i));
        lua_pushstring(L, entry.c_str());
        lua_rawseti(L, -2, index++);
    }
    WiFi.scanDelete();
    return 1;
}
static int wifiConnect(lua_State *L)
{
    const char *ssid = luaL_checkstring(L, 1);
    const char *pass = luaL_optstring(L, 2, "");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000)
        delay(100);
    lua_pushboolean(L, WiFi.status() == WL_CONNECTED);
    return 1;
}
static int wifiPing(lua_State *L)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    IPAddress host = WiFi.gatewayIP();
    WiFiClient client;
    unsigned long start = millis();
    bool ok = client.connect(host, 80, 2000);
    unsigned long ms = millis() - start;
    client.stop();
    lua_pushboolean(L, ok);
    lua_pushinteger(L, ok ? (lua_Integer)ms : -1);
    return 2;
}
static int wifiSpeedTest(lua_State *L)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        lua_pushinteger(L, -1);
        return 1;
    }
    const char *url = luaL_optstring(L, 1, "http://speedtest.tele2.net/100KB.zip");
    HTTPClient http;
    http.begin(url);
    int code = http.GET();
    if (code != 200)
    {
        http.end();
        lua_pushinteger(L, -1);
        return 1;
    }
    unsigned long start = millis();
    WiFiClient *stream = http.getStreamPtr();
    uint8_t buf[1024];
    size_t total = 0;
    while (http.connected() && (total < 128 * 1024))
    {
        size_t n = stream->readBytes(buf, sizeof(buf));
        if (n == 0)
            break;
        total += n;
    }
    unsigned long ms = millis() - start;
    http.end();
    lua_pushinteger(L, ms > 0 ? (lua_Integer)(total * 1000UL / ms / 1024UL) : 0);
    return 1;
}
static int httpGetHtml(lua_State *L)
{
    const char *url = luaL_checkstring(L, 1);
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    if (String(url).startsWith("https"))
        http.begin(client, url);
    else
        http.begin(url);
    int code = http.GET();
    if (code != 200)
    {
        http.end();
        lua_pushnil(L);
        return 1;
    }
    int len = http.getSize();
    if (len > 800 * 1024)
    {
        http.end();
        lua_pushnil(L);
        return 1;
    }
    String body = http.getString();
    http.end();
    if (body.length() > 800 * 1024)
    {
        lua_pushnil(L);
        return 1;
    }
    lua_pushlstring(L, body.c_str(), body.length());
    return 1;
}
static int httpDownload(lua_State *L)
{
    const char *url = luaL_checkstring(L, 1);
    const char *path = luaL_checkstring(L, 2);
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    if (String(url).startsWith("https"))
        http.begin(client, url);
    else
        http.begin(url);
    int code = http.GET();
    if (code != 200)
    {
        http.end();
        lua_pushboolean(L, false);
        return 1;
    }
    File f = LittleFS.open(path, "w");
    if (!f)
    {
        http.end();
        lua_pushboolean(L, false);
        return 1;
    }
    WiFiClient *stream = http.getStreamPtr();
    uint8_t buf[1024];
    while (http.connected())
    {
        size_t n = stream->readBytes(buf, sizeof(buf));
        if (n == 0)
            break;
        f.write(buf, n);
    }
    f.close();
    http.end();
    lua_pushboolean(L, true);
    return 1;
}
static int httpApi(lua_State *L)
{
    const char *url = luaL_checkstring(L, 1);
    const char *payload = luaL_optstring(L, 2, nullptr);
    const char *method = luaL_optstring(L, 3, "GET");
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    if (String(url).startsWith("https"))
        http.begin(client, url);
    else
        http.begin(url);
    if (payload && payload[0])
        http.addHeader("Content-Type", "application/json");
    int code;
    if (strcmp(method, "POST") == 0)
        code = http.POST((uint8_t *)payload, strlen(payload));
    else if (strcmp(method, "PUT") == 0)
        code = http.PUT((uint8_t *)payload, strlen(payload));
    else
        code = http.GET();
    if (code <= 0)
    {
        http.end();
        lua_pushnil(L);
        return 1;
    }
    String body = http.getString();
    http.end();
    lua_pushlstring(L, body.c_str(), body.length());
    return 1;
}
static int wifiDisconnect(lua_State *L)
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return 0;
}
static int wifiStatus(lua_State *L)
{
    lua_pushboolean(L, WiFi.status() == WL_CONNECTED);
    lua_pushstring(L, WiFi.localIP().toString().c_str());
    lua_pushstring(L, WiFi.SSID().c_str());
    lua_pushinteger(L, WiFi.RSSI());
    return 4;
}
static int wifiStaticIp(lua_State *L)
{
    const char *ip = luaL_checkstring(L, 1);
    const char *gw = luaL_checkstring(L, 2);
    const char *mask = luaL_checkstring(L, 3);
    const char *dns = luaL_optstring(L, 4, "8.8.8.8");
    IPAddress _ip, _gw, _mask, _dns;
    _ip.fromString(ip);
    _gw.fromString(gw);
    _mask.fromString(mask);
    _dns.fromString(dns);
    lua_pushboolean(L, WiFi.config(_ip, _gw, _mask, _dns));
    return 1;
}
static int wifiAp(lua_State *L)
{
    const char *ssid = luaL_checkstring(L, 1);
    const char *pass = luaL_optstring(L, 2, "");

    WiFi.mode(WIFI_AP);
    bool ok = (pass[0] == 0) ? WiFi.softAP(ssid) : WiFi.softAP(ssid, pass);
    lua_pushboolean(L, ok);
    lua_pushstring(L, WiFi.softAPIP().toString().c_str());
    return 2;
}
static int wifiBestAp(lua_State *L)
{
    int n = WiFi.scanNetworks();
    int best = -1, bestRssi = -1000;
    for (int i = 0; i < n; i++)
        if (WiFi.RSSI(i) > bestRssi)
        {
            bestRssi = WiFi.RSSI(i);
            best = i;
        }

    if (best < 0)
    {
        lua_pushnil(L);
        return 1;
    }
    lua_pushstring(L, WiFi.SSID(best).c_str());
    lua_pushinteger(L, bestRssi);
    WiFi.scanDelete();
    return 2;
}
static int lua_removeConfigLine(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    const char *key = luaL_checkstring(L, 2);

    File file = LittleFS.open(path, "r");
    if (!file)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    String content = "";
    String keyWithEquals = String(key) + "=";
    while (file.available())
    {
        String line = file.readStringUntil('\n');
        line.trim();
        if (!line.startsWith(keyWithEquals))
        {
            content += line + "\n";
        }
    }
    file.close();
    File outFile = LittleFS.open(path, "w");
    if (!outFile)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    outFile.print(content);
    outFile.close();
    lua_pushboolean(L, true);
    return 1;
}
static int lua_scanFiles(lua_State *L)
{
    const char *folder = luaL_checkstring(L, 1);
    const char *ext    = luaL_checkstring(L, 2);
    const char *name   = luaL_checkstring(L, 3);
    lua_newtable(L);
    int index = 1;
    File root = LittleFS.open(folder);
    if (!root) return 1;
    File file = root.openNextFile();
    while (file)
    {
        if (!file.isDirectory())
        {
            String fileName = file.name();
            String filePath = String(folder) + fileName;
            bool extMatch  = (ext[0]  == '*') || fileName.endsWith(ext);
            bool nameMatch = (name[0] == '*') || fileName.indexOf(name) != -1;
            if (extMatch && nameMatch)
            {
                lua_pushstring(L, filePath.c_str());
                lua_rawseti(L, -2, index++);
            }
        }
        file = root.openNextFile();
    }
    return 1;
}

static int lua_scanDirs(lua_State *L)
{
    const char *folder = luaL_checkstring(L, 1);    
    lua_newtable(L);    
    int index = 1;
    File root = LittleFS.open(folder);    
    if (!root || !root.isDirectory()) {    
        if (root) root.close();        
        return 1;        
    }
    while (true) {
        bool isDir = false;  
        String fullPath = root.getNextFileName(&isDir);  
        if (fullPath == "") break;
        if (isDir) {   
            lua_pushstring(L, fullPath.c_str());
            lua_rawseti(L, -2, index++);
        }
    }
    root.close();
    return 1;
}
static int lua_readLines(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);

    File file = LittleFS.open(path, "r");
    if (!file)
    {
        lua_newtable(L); 
        return 1;
    }

    lua_newtable(L);
    int index = 1;

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        line.trim();
        lua_pushstring(L, line.c_str());
        lua_rawseti(L, -2, index++);
    }

    file.close();
    return 1;
}
static int lua_writeConfig(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    const char *key = luaL_checkstring(L, 2);
    const char *value = luaL_checkstring(L, 3);

    File file = LittleFS.open(path, "a"); 
    if (!file)
    {
        file = LittleFS.open(path, "w"); 
    }
    file.println(String(key) + "=" + String(value));
    file.close();
    return 0;
}
static int lua_readConfig(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    const char *key = luaL_checkstring(L, 2);

    File file = LittleFS.open(path, "r");
    if (!file)
    {
        lua_pushnil(L);
        return 1;
    }

    String result = "";
    while (file.available())
    {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.startsWith(String(key) + "="))
        {
            result = line.substring(String(key).length() + 1);
            break;
        }
    }
    file.close();

    if (result.length() > 0)
    {
        lua_pushstring(L, result.c_str());
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}
static int run_file(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    if (LittleFS.exists(path))
    {
        File file = LittleFS.open(path, "r");
        String script = file.readString();
        Serial.println(lua.Lua_dostring(&script));
    }
    else
    {
        lua_pushnumber(L, 0);
    }
    return 1;
}
static int run_string(lua_State *L)
{
    const char *string = luaL_checkstring(L, 1);
    String script = String(string);
    String output = lua.Lua_dostring(&script);
    lua_pushstring(L, output.c_str());
    return 1;
}
static int free_ram(lua_State *L)
{
    lua_pushnumber(L, ESP.getFreeHeap());
    return 1;
}
static int free_psram(lua_State *L)
{
    lua_pushnumber(L, ESP.getFreePsram());
    return 1;
}
static int flash_size(lua_State *L)
{
    lua_pushnumber(L, ESP.getFlashChipSize());
    return 1;
}
static int ram_size(lua_State *L)
{
    lua_pushnumber(L, ESP.getHeapSize());
    return 1;
}
static int psram_size(lua_State *L)
{
    lua_pushnumber(L, ESP.getPsramSize());
    return 1;

}
static int temp(lua_State *L)
{
    lua_pushnumber(L, temperatureRead());
    return 1;
}
static int all_memory(lua_State *L)
{
    lua_pushnumber(L, LittleFS.totalBytes() / 1024);
    return 1;
}
static int used_memory(lua_State *L)
{
    lua_pushnumber(L, LittleFS.usedBytes() / 1024.0);
    return 1;
}
static int reboot(lua_State *L)
{
    ESP.restart();
    return 0;
}
static int renameFile(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    const char *newName = luaL_checkstring(L, 2);
    const char *slash = strrchr(path, '/');
    String newPath;
    if (slash)
    {
        newPath = String(path).substring(0, slash - path + 1) + newName;
    }
    else
    {
        newPath = newName;
    }
    lua_pushboolean(L, LittleFS.rename(path, newPath.c_str()));
    return 1;
}
static int moveFile(lua_State *L)
{
    const char *src = luaL_checkstring(L, 1);
    const char *dst = luaL_checkstring(L, 2);
    lua_pushboolean(L, LittleFS.rename(src, dst));
    return 1;
}
static int deleteFile(lua_State *L)
{
    lua_pushboolean(L, LittleFS.remove(luaL_checkstring(L, 1)));
    return 1;
}
static int createFile(lua_State *L)
{
    File f = LittleFS.open(luaL_checkstring(L, 1), "w");
    if (!f)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    f.close();
    lua_pushboolean(L, true);
    return 1;
}
static int copyFile(lua_State *L)
{
    const char *src = luaL_checkstring(L, 1);
    const char *dst = luaL_checkstring(L, 2);
    File s = LittleFS.open(src, "r");
    if (!s)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    File d = LittleFS.open(dst, "w");
    if (!d)
    {
        s.close();
        lua_pushboolean(L, false);
        return 1;
    }
    uint8_t buf[512];
    bool ok = true;
    while (s.available())
    {
        size_t n = s.read(buf, sizeof(buf));
        if (d.write(buf, n) != n)
        {
            ok = false;
            break;
        }
    }
    s.close();
    d.close();
    lua_pushboolean(L, ok);
    return 1;
}
static int writeFile(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    size_t len;
    const char *data = luaL_checklstring(L, 2, &len);

    File f = LittleFS.open(path, "a");
    if (!f)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    size_t n = f.write((const uint8_t *)data, len);
    f.close();
    lua_pushboolean(L, n == len);
    return 1;
}
static int clearFile(lua_State *L)
{
    File f = LittleFS.open(luaL_checkstring(L, 1), "w");
    if (!f)
    {
        lua_pushboolean(L, false);
        return 1;
    }
    f.close();
    lua_pushboolean(L, true);
    return 1;
}
static int fileSize(lua_State *L)
{
    File f = LittleFS.open(luaL_checkstring(L, 1), "r");
    if (!f)
    {
        lua_pushinteger(L, -1);
        return 1;
    }
    long sz = f.size();
    f.close();
    lua_pushinteger(L, sz);
    return 1;
}
static int createDir(lua_State *L)
{
    lua_pushboolean(L, LittleFS.mkdir(luaL_checkstring(L, 1)));
    return 1;
}
static int deleteDir(lua_State *L)
{
    lua_pushboolean(L, LittleFS.rmdir(luaL_checkstring(L, 1)));
    return 1;
}
static int ftp(lua_State *L)
{
     ftpSrv.handleFTP();
    return 0;
}
static int ftpBegin(lua_State *L)
{
  ftpSrv.begin(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
    return 0;
}
 
void makeMemory(LuaWrapper &lua)
{
lua.Lua_register("ftpBegin", (const lua_CFunction)&ftpBegin);
lua.Lua_register("ftpTick", (const lua_CFunction)&ftp);
lua.Lua_register("scanDirs", (const lua_CFunction)&lua_scanDirs);
    lua.Lua_register("scanFiles", (const lua_CFunction)&lua_scanFiles);
    lua.Lua_register("readLines", (const lua_CFunction)&lua_readLines);
    lua.Lua_register("writeConfig", (const lua_CFunction)&lua_writeConfig);
    lua.Lua_register("readConfig", (const lua_CFunction)&lua_readConfig);
    lua.Lua_register("runFile", (const lua_CFunction)&run_file);
    lua.Lua_register("removeConfigLine", (const lua_CFunction)&lua_removeConfigLine);
    lua.Lua_register("runString", (const lua_CFunction)&run_string);
    lua.Lua_register("free_ram", (const lua_CFunction)&free_ram);
    lua.Lua_register("free_psram", (const lua_CFunction)&free_psram);
    lua.Lua_register("flash_size", (const lua_CFunction)&flash_size);
    lua.Lua_register("ram_size", (const lua_CFunction)&ram_size);
    lua.Lua_register("psram_size", (const lua_CFunction)&psram_size);
    lua.Lua_register("temp", (const lua_CFunction)&temp);
    lua.Lua_register("all_memory", (const lua_CFunction)&all_memory);
    lua.Lua_register("used_memory", (const lua_CFunction)&used_memory);
    lua.Lua_register("reboot", (const lua_CFunction)&reboot);
    lua.Lua_register("renameFile", (const lua_CFunction)&renameFile);
    lua.Lua_register("moveFile", (const lua_CFunction)&moveFile);
    lua.Lua_register("deleteFile", (const lua_CFunction)&deleteFile);
    lua.Lua_register("createFile", (const lua_CFunction)&createFile);
    lua.Lua_register("copyFile", (const lua_CFunction)&copyFile);
    lua.Lua_register("writeFile", (const lua_CFunction)&writeFile);
    lua.Lua_register("clearFile", (const lua_CFunction)&clearFile);
    lua.Lua_register("fileSize", (const lua_CFunction)&fileSize);
    lua.Lua_register("createDir", (const lua_CFunction)&createDir);
    lua.Lua_register("deleteDir", (const lua_CFunction)&deleteDir);
    lua.Lua_register("wifiSetMac", (const lua_CFunction)&wifiSetMac);
    lua.Lua_register("wifiSetApMac", (const lua_CFunction)&wifiSetApMac);
    lua.Lua_register("wifiGetMac", (const lua_CFunction)&wifiGetMac);
    lua.Lua_register("wifiScan", (const lua_CFunction)&wifiScan);
    lua.Lua_register("wifiConnect", (const lua_CFunction)&wifiConnect);
    lua.Lua_register("wifiPing", (const lua_CFunction)&wifiPing);
    lua.Lua_register("wifiSpeedTest", (const lua_CFunction)&wifiSpeedTest);
    lua.Lua_register("httpGetHtml", (const lua_CFunction)&httpGetHtml);
    lua.Lua_register("httpDownload", (const lua_CFunction)&httpDownload);
    lua.Lua_register("httpApi", (const lua_CFunction)&httpApi);
    lua.Lua_register("wifiDisconnect", (const lua_CFunction)&wifiDisconnect);
    lua.Lua_register("wifiStatus", (const lua_CFunction)&wifiStatus);
    lua.Lua_register("wifiStaticIp", (const lua_CFunction)&wifiStaticIp);
    lua.Lua_register("wifiAp", (const lua_CFunction)&wifiAp);
    lua.Lua_register("wifiBestAp", (const lua_CFunction)&wifiBestAp);
}

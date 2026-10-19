#pragma once
#include <LuaWrapper.h>
#include "USBKeyboardHost.h"
static int get_keys(lua_State *lua_state)
{

    usb_kb_event_t evt;
    while (usb_kb_get_event(&evt))
    {
        switch (evt.type)
        {
        case USB_KB_CHAR:
            Serial.print("CHAR: ");
            if (evt.ch == '\r')
            {
                Serial.println("[ENTER]");
                lua_pushstring(lua_state, "[ENTER]");
            }
            else if (evt.ch == '\b')
            {
                Serial.println("[BKSP]");
                lua_pushstring(lua_state, "[BKSP]");
            }
            else if (evt.ch == '\t')
            {
                lua_pushstring(lua_state, "[TAB]");
                Serial.println("[TAB]");
            }

            else if (evt.ch == 0x1B)
            {
                Serial.println("[ESC]");
                lua_pushstring(lua_state, "[ESC]");
            }
            else
            {
                Serial.println(evt.ch);
                char buf[2] = {evt.ch, '\0'};
                lua_pushstring(lua_state, buf);
            }
            break;

        case USB_KB_SPECIAL:
            Serial.println(usb_kb_special_key_name(evt.special));
            lua_pushstring(lua_state, usb_kb_special_key_name(evt.special));
            break;

             case USB_KB_MODIFIER:
                 Serial.print("MOD: ");
                 Serial.print(usb_kb_special_key_name(evt.special));
                 Serial.printf(" | S=%d C=%d A=%d G=%d\n",
                     evt.shift, evt.ctrl, evt.alt, evt.gui);
                     if(evt.shift) lua_pushstring(lua_state, "[SHIFT]");
                     if(evt.alt) lua_pushstring(lua_state, "[ALT]");
                     if(evt.ctrl) lua_pushstring(lua_state, "[CTRL]");
                     if(evt.gui) lua_pushstring(lua_state, "[GUI]");
                 break;

        case USB_KB_NONE:
            lua_pushstring(lua_state, "none");
            break;
        }
    }
    if (evt.type == USB_KB_NONE)
    {
        lua_pushstring(lua_state, "none");
    }
    return 1;
}

void LuaKb(LuaWrapper &lua)
{
    lua.Lua_register("get_key", (const lua_CFunction)&get_keys);
}
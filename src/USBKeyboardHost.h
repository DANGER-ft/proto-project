#pragma once

#include <Arduino.h>
#include "usb/usb_host.h"
#include "hid_host.h"
#include "hid_usage_keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    USB_KB_CHAR,       
    USB_KB_SPECIAL,    
    USB_KB_MODIFIER,   
    USB_KB_NONE       
} usb_kb_key_type_t;

typedef enum {
    USB_KB_KEY_NONE = 0,
    USB_KB_KEY_ENTER,
    USB_KB_KEY_ESC,
    USB_KB_KEY_BACKSPACE,
    USB_KB_KEY_TAB,
    USB_KB_KEY_SPACE,
    USB_KB_KEY_CAPS_LOCK,
    USB_KB_KEY_NUM_LOCK,
    USB_KB_KEY_SCROLL_LOCK,
    USB_KB_KEY_INSERT,
    USB_KB_KEY_DELETE,
    USB_KB_KEY_HOME,
    USB_KB_KEY_END,
    USB_KB_KEY_PAGE_UP,
    USB_KB_KEY_PAGE_DOWN,
    USB_KB_KEY_UP,
    USB_KB_KEY_DOWN,
    USB_KB_KEY_LEFT,
    USB_KB_KEY_RIGHT,
    USB_KB_KEY_F1, USB_KB_KEY_F2, USB_KB_KEY_F3, USB_KB_KEY_F4,
    USB_KB_KEY_F5, USB_KB_KEY_F6, USB_KB_KEY_F7, USB_KB_KEY_F8,
    USB_KB_KEY_F9, USB_KB_KEY_F10, USB_KB_KEY_F11, USB_KB_KEY_F12,
    USB_KB_KEY_PRINT_SCREEN,
    USB_KB_KEY_PAUSE,
    USB_KB_KEY_MENU,
    USB_KB_KEY_LCTRL, USB_KB_KEY_LSHIFT, USB_KB_KEY_LALT, USB_KB_KEY_LGUI,
    USB_KB_KEY_RCTRL, USB_KB_KEY_RSHIFT, USB_KB_KEY_RALT, USB_KB_KEY_RGUI,
} usb_kb_special_key_t;


typedef struct {
    usb_kb_key_type_t type;
    char ch;
    usb_kb_special_key_t special;
    bool shift;
    bool ctrl;
    bool alt;
    bool gui;
} usb_kb_event_t;


void usb_kb_init(void);
bool usb_kb_get_event(usb_kb_event_t *event);
bool usb_kb_wait_event(usb_kb_event_t *event, uint32_t timeout_ms);
bool usb_kb_is_shift_pressed(void);
bool usb_kb_is_ctrl_pressed(void);
bool usb_kb_is_alt_pressed(void);
bool usb_kb_is_gui_pressed(void);
const char* usb_kb_special_key_name(usb_kb_special_key_t key);

#ifdef __cplusplus
}
#endif

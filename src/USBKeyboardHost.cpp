#include "USBKeyboardHost.h"

static const char *TAG = "USB_KB";

static const uint8_t keymap_normal[] = {
    0, 0, 0, 0,
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    13, 27, 8, 9, 32, '-', '=', '[', ']', 92, 0, ';', 39, '`', ',', '.', '/',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const uint8_t keymap_shift[] = {
    0, 0, 0, 0,
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z',
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    13, 27, 8, 9, 32, '_', '+', '{', '}', '|', 0, ':', '"', '~', '<', '>', '?',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};


static usb_kb_special_key_t special_map[128];
static bool special_map_initialized = false;

static void init_special_map(void) {
    if (special_map_initialized) return;
    memset(special_map, 0, sizeof(special_map));
    special_map[0x28] = USB_KB_KEY_ENTER;
    special_map[0x29] = USB_KB_KEY_ESC;
    special_map[0x2A] = USB_KB_KEY_BACKSPACE;
    special_map[0x2B] = USB_KB_KEY_TAB;
    special_map[0x2C] = USB_KB_KEY_SPACE;
    special_map[0x39] = USB_KB_KEY_CAPS_LOCK;
    special_map[0x47] = USB_KB_KEY_SCROLL_LOCK;
    special_map[0x49] = USB_KB_KEY_INSERT;
    special_map[0x4A] = USB_KB_KEY_HOME;
    special_map[0x4B] = USB_KB_KEY_PAGE_UP;
    special_map[0x4C] = USB_KB_KEY_DELETE;
    special_map[0x4D] = USB_KB_KEY_END;
    special_map[0x4E] = USB_KB_KEY_PAGE_DOWN;
    special_map[0x4F] = USB_KB_KEY_RIGHT;
    special_map[0x50] = USB_KB_KEY_LEFT;
    special_map[0x51] = USB_KB_KEY_DOWN;
    special_map[0x52] = USB_KB_KEY_UP;
    special_map[0x53] = USB_KB_KEY_NUM_LOCK;
    special_map[0x54] = USB_KB_KEY_PRINT_SCREEN;
    special_map[0x55] = USB_KB_KEY_PAUSE;
    special_map[0x65] = USB_KB_KEY_MENU;
    special_map[0x3A] = USB_KB_KEY_F1;
    special_map[0x3B] = USB_KB_KEY_F2;
    special_map[0x3C] = USB_KB_KEY_F3;
    special_map[0x3D] = USB_KB_KEY_F4;
    special_map[0x3E] = USB_KB_KEY_F5;
    special_map[0x3F] = USB_KB_KEY_F6;
    special_map[0x40] = USB_KB_KEY_F7;
    special_map[0x41] = USB_KB_KEY_F8;
    special_map[0x42] = USB_KB_KEY_F9;
    special_map[0x43] = USB_KB_KEY_F10;
    special_map[0x44] = USB_KB_KEY_F11;
    special_map[0x45] = USB_KB_KEY_F12;
    special_map_initialized = true;
}

static const usb_kb_special_key_t modifier_map[8] = {
    USB_KB_KEY_LCTRL, USB_KB_KEY_LSHIFT, USB_KB_KEY_LALT, USB_KB_KEY_LGUI,
    USB_KB_KEY_RCTRL, USB_KB_KEY_RSHIFT, USB_KB_KEY_RALT, USB_KB_KEY_RGUI,
};

static QueueHandle_t event_queue = NULL;
static uint8_t prev_report[8] = {0};
static uint8_t current_modifiers = 0;

static inline bool kb_is_shift(uint8_t m) { return (m & HID_LEFT_SHIFT) || (m & HID_RIGHT_SHIFT); }
static inline bool kb_is_ctrl(uint8_t m)  { return (m & HID_LEFT_CONTROL) || (m & HID_RIGHT_CONTROL); }
static inline bool kb_is_alt(uint8_t m)   { return (m & HID_LEFT_ALT) || (m & HID_RIGHT_ALT); }
static inline bool kb_is_gui(uint8_t m)   { return (m & HID_LEFT_GUI) || (m & HID_RIGHT_GUI); }

static void push_event(const usb_kb_event_t *evt) {
    if (event_queue) xQueueSend(event_queue, evt, 0);
}

static void process_modifiers(uint8_t modifier) {
    uint8_t changed = modifier ^ current_modifiers;
    current_modifiers = modifier;
    for (int i = 0; i < 8; i++) {
        if (changed & (1 << i)) {
            usb_kb_event_t evt;
            evt.type = USB_KB_MODIFIER;
            evt.ch = 0;
            evt.special = modifier_map[i];
            evt.shift = kb_is_shift(modifier);
            evt.ctrl = kb_is_ctrl(modifier);
            evt.alt = kb_is_alt(modifier);
            evt.gui = kb_is_gui(modifier);
            push_event(&evt);
        }
    }
}

static void process_key(uint8_t keycode, uint8_t modifier) {
    if (keycode == 0 || keycode >= 128) return;
    if (!special_map_initialized) init_special_map();
    
    bool shift = kb_is_shift(modifier);
    
    usb_kb_event_t evt;
    evt.shift = shift;
    evt.ctrl = kb_is_ctrl(modifier);
    evt.alt = kb_is_alt(modifier);
    evt.gui = kb_is_gui(modifier);
    
    if (keycode >= 0xE0 && keycode <= 0xE7) {
        evt.type = USB_KB_MODIFIER;
        evt.special = modifier_map[keycode - 0xE0];
        push_event(&evt);
        return;
    }
    
    if (special_map[keycode] != USB_KB_KEY_NONE) {
        evt.type = USB_KB_SPECIAL;
        evt.special = special_map[keycode];
        push_event(&evt);
        return;
    }
    
    char ch = (char)(shift ? keymap_shift[keycode] : keymap_normal[keycode]);
    if (ch) {
        evt.type = USB_KB_CHAR;
        evt.ch = ch;
        evt.special = USB_KB_KEY_NONE;
        push_event(&evt);
    }
}

static void process_report(const uint8_t *report, size_t len) {
    if (len < 8) return;
    uint8_t modifier = report[0];
    uint8_t *keys = (uint8_t*)&report[2];
    process_modifiers(modifier);
    for (int i = 0; i < 6; i++) {
        uint8_t key = keys[i];
        if (key == 0) continue;
        bool was_pressed = false;
        for (int j = 0; j < 6; j++) {
            if (prev_report[j + 2] == key) { was_pressed = true; break; }
        }
        if (!was_pressed) process_key(key, modifier);
    }
    memcpy(prev_report, report, 8);
}

static void hid_host_interface_callback(hid_host_device_handle_t hid_device_handle,
                                        const hid_host_interface_event_t event, void *arg) {
    uint8_t data[64] = {0};
    size_t data_length = 0;
    hid_host_dev_params_t dev_params;
    if (hid_host_device_get_params(hid_device_handle, &dev_params) != ESP_OK) return;

    switch (event) {
    case HID_HOST_INTERFACE_EVENT_INPUT_REPORT:
        if (hid_host_device_get_raw_input_report_data(hid_device_handle, data, 64, &data_length) != ESP_OK) return;
        if (dev_params.sub_class == HID_SUBCLASS_BOOT_INTERFACE && dev_params.proto == HID_PROTOCOL_KEYBOARD)
            process_report(data, data_length);
        break;
    case HID_HOST_INTERFACE_EVENT_DISCONNECTED:
        memset(prev_report, 0, 8); current_modifiers = 0;
        hid_host_device_close(hid_device_handle);
        break;
    default: break;
    }
}

static void hid_host_device_event(hid_host_device_handle_t hid_device_handle,
                                  const hid_host_driver_event_t event, void *arg) {
    hid_host_dev_params_t dev_params;
    if (hid_host_device_get_params(hid_device_handle, &dev_params) != ESP_OK) return;

    if (event == HID_HOST_DRIVER_EVENT_CONNECTED) {
        ESP_LOGI(TAG, "Keyboard connected (proto=%d, subclass=%d)", dev_params.proto, dev_params.sub_class);
        const hid_host_device_config_t dev_config = {
            .callback = hid_host_interface_callback, .callback_arg = NULL
        };
        if (hid_host_device_open(hid_device_handle, &dev_config) != ESP_OK) {
            ESP_LOGE(TAG, "Open failed"); return;
        }
        hid_class_request_set_protocol(hid_device_handle, HID_REPORT_PROTOCOL_BOOT);
        hid_class_request_set_idle(hid_device_handle, 0, 0);
        if (hid_host_device_start(hid_device_handle) != ESP_OK) {
            ESP_LOGE(TAG, "Start failed");
            hid_host_device_close(hid_device_handle);
            return;
        }
        ESP_LOGI(TAG, "Keyboard started");
    }
}

static void usb_lib_task(void *arg) {
    const usb_host_config_t host_config = {
        .skip_phy_setup = false, .intr_flags = ESP_INTR_FLAG_LEVEL1,
    };
    ESP_ERROR_CHECK(usb_host_install(&host_config));
    xTaskNotifyGive((TaskHandle_t)arg);
    while (true) {
        uint32_t event_flags;
        usb_host_lib_handle_events(portMAX_DELAY, &event_flags);
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) usb_host_device_free_all();
    }
}



void usb_kb_init(void) {
    event_queue = xQueueCreate(32, sizeof(usb_kb_event_t));
    xTaskCreatePinnedToCore(usb_lib_task, "usb_events", 4096,
                            xTaskGetCurrentTaskHandle(), 2, NULL, 0);
    ulTaskNotifyTake(false, 1000);
    const hid_host_driver_config_t hid_config = {
        .create_background_task = true, .task_priority = 5,
        .stack_size = 4096, .core_id = 0,
        .callback = hid_host_device_event, .callback_arg = NULL
    };
    ESP_ERROR_CHECK(hid_host_install(&hid_config));
}

bool usb_kb_get_event(usb_kb_event_t *event) {
    if (!event_queue) return false;
    return xQueueReceive(event_queue, event, 0) == pdTRUE;
}

bool usb_kb_wait_event(usb_kb_event_t *event, uint32_t timeout_ms) {
    if (!event_queue) return false;
    TickType_t ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xQueueReceive(event_queue, event, ticks) == pdTRUE;
}

bool usb_kb_is_shift_pressed(void) { return kb_is_shift(current_modifiers); }
bool usb_kb_is_ctrl_pressed(void)  { return kb_is_ctrl(current_modifiers); }
bool usb_kb_is_alt_pressed(void)   { return kb_is_alt(current_modifiers); }
bool usb_kb_is_gui_pressed(void)   { return kb_is_gui(current_modifiers); }

const char* usb_kb_special_key_name(usb_kb_special_key_t key) {
    switch (key) {
        case USB_KB_KEY_NONE:         return "NONE";
        case USB_KB_KEY_ENTER:        return "ENTER";
        case USB_KB_KEY_ESC:          return "ESC";
        case USB_KB_KEY_BACKSPACE:    return "BACKSPACE";
        case USB_KB_KEY_TAB:          return "TAB";
        case USB_KB_KEY_SPACE:        return "SPACE";
        case USB_KB_KEY_CAPS_LOCK:    return "CAPS_LOCK";
        case USB_KB_KEY_NUM_LOCK:     return "NUM_LOCK";
        case USB_KB_KEY_SCROLL_LOCK:  return "SCROLL_LOCK";
        case USB_KB_KEY_INSERT:       return "INSERT";
        case USB_KB_KEY_DELETE:       return "DELETE";
        case USB_KB_KEY_HOME:         return "HOME";
        case USB_KB_KEY_END:          return "END";
        case USB_KB_KEY_PAGE_UP:      return "PAGE_UP";
        case USB_KB_KEY_PAGE_DOWN:    return "PAGE_DOWN";
        case USB_KB_KEY_UP:           return "UP";
        case USB_KB_KEY_DOWN:         return "DOWN";
        case USB_KB_KEY_LEFT:         return "LEFT";
        case USB_KB_KEY_RIGHT:        return "RIGHT";
        case USB_KB_KEY_F1:           return "F1";
        case USB_KB_KEY_F2:           return "F2";
        case USB_KB_KEY_F3:           return "F3";
        case USB_KB_KEY_F4:           return "F4";
        case USB_KB_KEY_F5:           return "F5";
        case USB_KB_KEY_F6:           return "F6";
        case USB_KB_KEY_F7:           return "F7";
        case USB_KB_KEY_F8:           return "F8";
        case USB_KB_KEY_F9:           return "F9";
        case USB_KB_KEY_F10:          return "F10";
        case USB_KB_KEY_F11:          return "F11";
        case USB_KB_KEY_F12:          return "F12";
        case USB_KB_KEY_PRINT_SCREEN: return "PRINT_SCREEN";
        case USB_KB_KEY_PAUSE:        return "PAUSE";
        case USB_KB_KEY_MENU:         return "MENU";
        case USB_KB_KEY_LCTRL:        return "LCTRL";
        case USB_KB_KEY_LSHIFT:       return "LSHIFT";
        case USB_KB_KEY_LALT:         return "LALT";
        case USB_KB_KEY_LGUI:         return "LGUI/WIN";
        case USB_KB_KEY_RCTRL:        return "RCTRL";
        case USB_KB_KEY_RSHIFT:       return "RSHIFT";
        case USB_KB_KEY_RALT:         return "RALT";
        case USB_KB_KEY_RGUI:         return "RGUI/WIN";
        default:                      return "UNKNOWN";
    }
}

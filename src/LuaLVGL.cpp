#include "LuaLVGL.h"
#include <lvgl.h>

static lv_obj_t* lv_check(lua_State *L, int idx) {
    if (lua_isnoneornil(L, idx)) return NULL;
    return (lv_obj_t*)lua_touserdata(L, idx);
}

static void lv_push(lua_State *L, lv_obj_t *obj) {
    lua_pushlightuserdata(L, obj);
}

static int _lv_scr_act(lua_State *L) {
    lv_push(L, lv_scr_act());
    return 1;
}

static int _lv_obj_create(lua_State *L) {
    lv_obj_t *parent = lv_check(L, 1);
    lv_push(L, lv_obj_create(parent));
    return 1;
}

static int _lv_btn_create(lua_State *L) {
    lv_push(L, lv_btn_create(lv_check(L, 1)));
    return 1;
}

static int _lv_label_create(lua_State *L) {
    lv_push(L, lv_label_create(lv_check(L, 1)));
    return 1;
}

static int _lv_list_create(lua_State *L) {
    lv_push(L, lv_list_create(lv_check(L, 1)));
    return 1;
}

static int _lv_slider_create(lua_State *L) {
    lv_push(L, lv_slider_create(lv_check(L, 1)));
    return 1;
}

static int _lv_bar_create(lua_State *L) {
    lv_push(L, lv_bar_create(lv_check(L, 1)));
    return 1;
}

static int _lv_switch_create(lua_State *L) {
    lv_push(L, lv_switch_create(lv_check(L, 1)));
    return 1;
}

static int _lv_checkbox_create(lua_State *L) {
    lv_push(L, lv_checkbox_create(lv_check(L, 1)));
    return 1;
}

static int _lv_dropdown_create(lua_State *L) {
    lv_push(L, lv_dropdown_create(lv_check(L, 1)));
    return 1;
}

static int _lv_img_create(lua_State *L) {
    lv_push(L, lv_img_create(lv_check(L, 1)));
    return 1;
}

static int _lv_arc_create(lua_State *L) {
    lv_push(L, lv_arc_create(lv_check(L, 1)));
    return 1;
}

static int _lv_chart_create(lua_State *L) {
    lv_push(L, lv_chart_create(lv_check(L, 1)));
    return 1;
}

static int _lv_table_create(lua_State *L) {
    lv_push(L, lv_table_create(lv_check(L, 1)));
    return 1;
}

static int _lv_calendar_create(lua_State *L) {
    lv_push(L, lv_calendar_create(lv_check(L, 1)));
    return 1;
}

static int _lv_spinner_create(lua_State *L) {
    lv_obj_t *parent = lv_check(L, 1);
    uint32_t time = luaL_optinteger(L, 2, 1000);
    uint32_t arc_length = luaL_optinteger(L, 3, 60);
    lv_push(L, lv_spinner_create(parent, time, arc_length));
    return 1;
}

static int _lv_win_create(lua_State *L) {
    lv_obj_t *parent = lv_check(L, 1);
    int title_h = luaL_optinteger(L, 2, 20);
    lv_push(L, lv_win_create(parent, title_h));
    return 1;
}

static int _lv_msgbox_create(lua_State *L) {
    lv_obj_t *parent = lv_check(L, 1);
    const char *title = luaL_optstring(L, 2, NULL);
    const char *text  = luaL_optstring(L, 3, NULL);
    lv_push(L, lv_msgbox_create(parent, title, text, NULL, true));
    return 1;
}

static int _lv_tabview_create(lua_State *L) {
    lv_obj_t *parent = lv_check(L, 1);
    lv_dir_t dir = (lv_dir_t)luaL_optinteger(L, 2, LV_DIR_TOP);
    int size = luaL_optinteger(L, 3, 30);
    lv_push(L, lv_tabview_create(parent, dir, size));
    return 1;
}

static int _lv_obj_set_pos(lua_State *L) {
    lv_obj_set_pos(lv_check(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}

static int _lv_obj_set_x(lua_State *L) {
    lv_obj_set_x(lv_check(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _lv_obj_set_y(lua_State *L) {
    lv_obj_set_y(lv_check(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _lv_obj_set_size(lua_State *L) {
    lv_obj_set_size(lv_check(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}

static int _lv_obj_set_width(lua_State *L) {
    lv_obj_set_width(lv_check(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _lv_obj_set_height(lua_State *L) {
    lv_obj_set_height(lv_check(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _lv_obj_align(lua_State *L) {
    lv_obj_align(lv_check(L, 1), (lv_align_t)luaL_checkinteger(L, 2),
                 luaL_optinteger(L, 3, 0), luaL_optinteger(L, 4, 0));
    return 0;
}

static int _lv_obj_center(lua_State *L) {
    lv_obj_center(lv_check(L, 1));
    return 0;
}

static int _lv_obj_align_to(lua_State *L) {
    lv_obj_align_to(lv_check(L, 1), lv_check(L, 2),
                    (lv_align_t)luaL_checkinteger(L, 3),
                    luaL_optinteger(L, 4, 0), luaL_optinteger(L, 5, 0));
    return 0;
}

static int _lv_obj_get_x(lua_State *L) {
    lua_pushinteger(L, lv_obj_get_x(lv_check(L, 1)));
    return 1;
}

static int _lv_obj_get_y(lua_State *L) {
    lua_pushinteger(L, lv_obj_get_y(lv_check(L, 1)));
    return 1;
}

static int _lv_obj_get_width(lua_State *L) {
    lua_pushinteger(L, lv_obj_get_width(lv_check(L, 1)));
    return 1;
}

static int _lv_obj_get_height(lua_State *L) {
    lua_pushinteger(L, lv_obj_get_height(lv_check(L, 1)));
    return 1;
}

static int _lv_obj_set_style_bg_color(lua_State *L) {
    lv_obj_t *o = lv_check(L, 1);
    uint32_t color = luaL_checkinteger(L, 2);
    lv_obj_set_style_bg_color(o, lv_color_hex(color), 0);
    return 0;
}

static int _lv_obj_set_style_bg_opa(lua_State *L) {
    lv_obj_set_style_bg_opa(lv_check(L, 1), luaL_checkinteger(L, 2), 0);
    return 0;
}

static int _lv_obj_set_style_text_color(lua_State *L) {
    lv_obj_set_style_text_color(lv_check(L, 1), lv_color_hex(luaL_checkinteger(L, 2)), 0);
    return 0;
}

static int _lv_obj_set_style_radius(lua_State *L) {
    lv_obj_set_style_radius(lv_check(L, 1), luaL_checkinteger(L, 2), 0);
    return 0;
}

static int _lv_obj_set_style_border_width(lua_State *L) {
    lv_obj_set_style_border_width(lv_check(L, 1), luaL_checkinteger(L, 2), 0);
    return 0;
}

static int _lv_obj_set_style_border_color(lua_State *L) {
    lv_obj_set_style_border_color(lv_check(L, 1), lv_color_hex(luaL_checkinteger(L, 2)), 0);
    return 0;
}

static int _lv_obj_set_style_pad_all(lua_State *L) {
    lv_obj_set_style_pad_all(lv_check(L, 1), luaL_checkinteger(L, 2), 0);
    return 0;
}

static int _lv_obj_set_style_pad_row(lua_State *L) {
    lv_obj_set_style_pad_row(lv_check(L, 1), luaL_checkinteger(L, 2), 0);
    return 0;
}

static int _lv_obj_set_style_pad_column(lua_State *L) {
    lv_obj_set_style_pad_column(lv_check(L, 1), luaL_checkinteger(L, 2), 0);
    return 0;
}

static int _lv_obj_set_style_opa(lua_State *L) {
    lv_obj_set_style_opa(lv_check(L, 1), luaL_checkinteger(L, 2), 0);
    return 0;
}

static int _lv_obj_set_style_text_font(lua_State *L) {
    lv_obj_set_style_text_font(lv_check(L, 1),
                               (const lv_font_t*)lua_touserdata(L, 2), 0);
    return 0;
}

static int _lv_label_set_text(lua_State *L) {
    lv_label_set_text(lv_check(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _lv_label_set_long_mode(lua_State *L) {
    lv_label_set_long_mode(lv_check(L, 1), (lv_label_long_mode_t)luaL_checkinteger(L, 2));
    return 0;
}


static int _lv_list_add_btn(lua_State *L) {
    lv_obj_t *list = lv_check(L, 1);
    const char *icon = lua_isnoneornil(L, 2) ? NULL : luaL_checkstring(L, 2);
    const char *text = luaL_checkstring(L, 3);
    lv_push(L, lv_list_add_btn(list, icon, text));
    return 1;
}

static int _lv_list_add_text(lua_State *L) {
    lv_push(L, lv_list_add_text(lv_check(L, 1), luaL_checkstring(L, 2)));
    return 1;
}


static int _lv_slider_set_value(lua_State *L) {
    lv_slider_set_value(lv_check(L, 1), luaL_checkinteger(L, 2),
                        lua_toboolean(L, 3) ? LV_ANIM_ON : LV_ANIM_OFF);
    return 0;
}

static int _lv_slider_set_range(lua_State *L) {
    lv_slider_set_range(lv_check(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}

static int _lv_slider_get_value(lua_State *L) {
    lua_pushinteger(L, lv_slider_get_value(lv_check(L, 1)));
    return 1;
}

static int _lv_bar_set_value(lua_State *L) {
    lv_bar_set_value(lv_check(L, 1), luaL_checkinteger(L, 2),
                     lua_toboolean(L, 3) ? LV_ANIM_ON : LV_ANIM_OFF);
    return 0;
}

static int _lv_bar_set_range(lua_State *L) {
    lv_bar_set_range(lv_check(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}

static int _lv_bar_get_value(lua_State *L) {
    lua_pushinteger(L, lv_bar_get_value(lv_check(L, 1)));
    return 1;
}

static int _lv_switch_set_state(lua_State *L) {
    lv_obj_t *sw = lv_check(L, 1);
    if (lua_toboolean(L, 2)) lv_obj_add_state(sw, LV_STATE_CHECKED);
    else lv_obj_clear_state(sw, LV_STATE_CHECKED);
    return 0;
}

static int _lv_checkbox_set_text(lua_State *L) {
    lv_checkbox_set_text(lv_check(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _lv_dropdown_set_options(lua_State *L) {
    lv_dropdown_set_options(lv_check(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _lv_dropdown_set_selected(lua_State *L) {
    lv_dropdown_set_selected(lv_check(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _lv_dropdown_get_selected(lua_State *L) {
    lua_pushinteger(L, lv_dropdown_get_selected(lv_check(L, 1)));
    return 1;
}

static int _lv_arc_set_value(lua_State *L) {
    lv_arc_set_value(lv_check(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _lv_arc_get_value(lua_State *L) {
    lua_pushinteger(L, lv_arc_get_value(lv_check(L, 1)));
    return 1;
}


static int _lv_img_set_src(lua_State *L) {
    lv_img_set_src(lv_check(L, 1), luaL_checkstring(L, 2));
    return 0;
}


static int _lv_obj_del(lua_State *L) {
    lv_obj_del(lv_check(L, 1));
    return 0;
}

static int _lv_obj_clean(lua_State *L) {
    lv_obj_clean(lv_check(L, 1));
    return 0;
}

static int _lv_obj_get_child(lua_State *L) {
    lv_push(L, lv_obj_get_child(lv_check(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _lv_obj_get_child_cnt(lua_State *L) {
    lua_pushinteger(L, lv_obj_get_child_cnt(lv_check(L, 1)));
    return 1;
}

static int _lv_obj_add_flag(lua_State *L) {
    lv_obj_add_flag(lv_check(L, 1), (lv_obj_flag_t)luaL_checkinteger(L, 2));
    return 0;
}

static int _lv_obj_clear_flag(lua_State *L) {
    lv_obj_clear_flag(lv_check(L, 1), (lv_obj_flag_t)luaL_checkinteger(L, 2));
    return 0;
}


static int _lv_scr_load(lua_State *L) {
    lv_scr_load(lv_check(L, 1));
    return 0;
}

static int _lv_scr_load_anim(lua_State *L) {
    lv_scr_load_anim(lv_check(L, 1),
                     (lv_scr_load_anim_t)luaL_checkinteger(L, 2),
                     luaL_optinteger(L, 3, 300),
                     luaL_optinteger(L, 4, 0),
                     lua_toboolean(L, 5));
    return 0;
}


static int _lv_obj_set_flex_flow(lua_State *L) {
    lv_obj_set_flex_flow(lv_check(L, 1), (lv_flex_flow_t)luaL_checkinteger(L, 2));
    return 0;
}


extern void lvgl_bridge_tick();

static int _lv_timer_handler(lua_State *L) {
    lvgl_bridge_tick();
    return 0;
}

static int _lv_tick_inc(lua_State *L) {
    lv_tick_inc(luaL_checkinteger(L, 1));
    return 0;
}

void LuaLVGL(LuaWrapper &lua) {
    lua.Lua_register("lv_scr_act",          (const lua_CFunction)&_lv_scr_act);
    lua.Lua_register("lv_obj_create",       (const lua_CFunction)&_lv_obj_create);
    lua.Lua_register("lv_btn_create",       (const lua_CFunction)&_lv_btn_create);
    lua.Lua_register("lv_label_create",     (const lua_CFunction)&_lv_label_create);
    lua.Lua_register("lv_list_create",      (const lua_CFunction)&_lv_list_create);
    lua.Lua_register("lv_slider_create",    (const lua_CFunction)&_lv_slider_create);
    lua.Lua_register("lv_bar_create",       (const lua_CFunction)&_lv_bar_create);
    lua.Lua_register("lv_switch_create",    (const lua_CFunction)&_lv_switch_create);
    lua.Lua_register("lv_checkbox_create",  (const lua_CFunction)&_lv_checkbox_create);
    lua.Lua_register("lv_dropdown_create",  (const lua_CFunction)&_lv_dropdown_create);
    lua.Lua_register("lv_img_create",       (const lua_CFunction)&_lv_img_create);
    lua.Lua_register("lv_arc_create",       (const lua_CFunction)&_lv_arc_create);
    lua.Lua_register("lv_chart_create",     (const lua_CFunction)&_lv_chart_create);
    lua.Lua_register("lv_table_create",     (const lua_CFunction)&_lv_table_create);
    lua.Lua_register("lv_calendar_create",  (const lua_CFunction)&_lv_calendar_create);
    lua.Lua_register("lv_spinner_create",   (const lua_CFunction)&_lv_spinner_create);
    lua.Lua_register("lv_win_create",       (const lua_CFunction)&_lv_win_create);
    lua.Lua_register("lv_msgbox_create",    (const lua_CFunction)&_lv_msgbox_create);
    lua.Lua_register("lv_tabview_create",   (const lua_CFunction)&_lv_tabview_create);

    lua.Lua_register("lv_obj_set_pos",      (const lua_CFunction)&_lv_obj_set_pos);
    lua.Lua_register("lv_obj_set_x",        (const lua_CFunction)&_lv_obj_set_x);
    lua.Lua_register("lv_obj_set_y",        (const lua_CFunction)&_lv_obj_set_y);
    lua.Lua_register("lv_obj_set_size",     (const lua_CFunction)&_lv_obj_set_size);
    lua.Lua_register("lv_obj_set_width",    (const lua_CFunction)&_lv_obj_set_width);
    lua.Lua_register("lv_obj_set_height",   (const lua_CFunction)&_lv_obj_set_height);
    lua.Lua_register("lv_obj_align",        (const lua_CFunction)&_lv_obj_align);
    lua.Lua_register("lv_obj_center",       (const lua_CFunction)&_lv_obj_center);
    lua.Lua_register("lv_obj_align_to",     (const lua_CFunction)&_lv_obj_align_to);
    lua.Lua_register("lv_obj_get_x",        (const lua_CFunction)&_lv_obj_get_x);
    lua.Lua_register("lv_obj_get_y",        (const lua_CFunction)&_lv_obj_get_y);
    lua.Lua_register("lv_obj_get_width",    (const lua_CFunction)&_lv_obj_get_width);
    lua.Lua_register("lv_obj_get_height",   (const lua_CFunction)&_lv_obj_get_height);


    lua.Lua_register("lv_obj_set_style_bg_color",       (const lua_CFunction)&_lv_obj_set_style_bg_color);
    lua.Lua_register("lv_obj_set_style_bg_opa",         (const lua_CFunction)&_lv_obj_set_style_bg_opa);
    lua.Lua_register("lv_obj_set_style_text_color",     (const lua_CFunction)&_lv_obj_set_style_text_color);
    lua.Lua_register("lv_obj_set_style_radius",         (const lua_CFunction)&_lv_obj_set_style_radius);
    lua.Lua_register("lv_obj_set_style_border_width",   (const lua_CFunction)&_lv_obj_set_style_border_width);
    lua.Lua_register("lv_obj_set_style_border_color",   (const lua_CFunction)&_lv_obj_set_style_border_color);
    lua.Lua_register("lv_obj_set_style_pad_all",        (const lua_CFunction)&_lv_obj_set_style_pad_all);
    lua.Lua_register("lv_obj_set_style_pad_row",        (const lua_CFunction)&_lv_obj_set_style_pad_row);
    lua.Lua_register("lv_obj_set_style_pad_column",     (const lua_CFunction)&_lv_obj_set_style_pad_column);
    lua.Lua_register("lv_obj_set_style_opa",            (const lua_CFunction)&_lv_obj_set_style_opa);
    lua.Lua_register("lv_obj_set_style_text_font",      (const lua_CFunction)&_lv_obj_set_style_text_font);

    lua.Lua_register("lv_label_set_text",       (const lua_CFunction)&_lv_label_set_text);
    lua.Lua_register("lv_label_set_long_mode",  (const lua_CFunction)&_lv_label_set_long_mode);


    lua.Lua_register("lv_list_add_btn",     (const lua_CFunction)&_lv_list_add_btn);
    lua.Lua_register("lv_list_add_text",    (const lua_CFunction)&_lv_list_add_text);


    lua.Lua_register("lv_slider_set_value",     (const lua_CFunction)&_lv_slider_set_value);
    lua.Lua_register("lv_slider_set_range",     (const lua_CFunction)&_lv_slider_set_range);
    lua.Lua_register("lv_slider_get_value",     (const lua_CFunction)&_lv_slider_get_value);
    lua.Lua_register("lv_bar_set_value",        (const lua_CFunction)&_lv_bar_set_value);
    lua.Lua_register("lv_bar_set_range",        (const lua_CFunction)&_lv_bar_set_range);
    lua.Lua_register("lv_bar_get_value",        (const lua_CFunction)&_lv_bar_get_value);
    lua.Lua_register("lv_switch_set_state",     (const lua_CFunction)&_lv_switch_set_state);
    lua.Lua_register("lv_checkbox_set_text",    (const lua_CFunction)&_lv_checkbox_set_text);
    lua.Lua_register("lv_dropdown_set_options", (const lua_CFunction)&_lv_dropdown_set_options);
    lua.Lua_register("lv_dropdown_set_selected",(const lua_CFunction)&_lv_dropdown_set_selected);
    lua.Lua_register("lv_dropdown_get_selected",(const lua_CFunction)&_lv_dropdown_get_selected);
    lua.Lua_register("lv_arc_set_value",        (const lua_CFunction)&_lv_arc_set_value);
    lua.Lua_register("lv_arc_get_value",        (const lua_CFunction)&_lv_arc_get_value);


    lua.Lua_register("lv_img_set_src",      (const lua_CFunction)&_lv_img_set_src);


    lua.Lua_register("lv_obj_del",          (const lua_CFunction)&_lv_obj_del);
    lua.Lua_register("lv_obj_clean",        (const lua_CFunction)&_lv_obj_clean);
    lua.Lua_register("lv_obj_get_child",    (const lua_CFunction)&_lv_obj_get_child);
    lua.Lua_register("lv_obj_get_child_cnt",(const lua_CFunction)&_lv_obj_get_child_cnt);
    lua.Lua_register("lv_obj_add_flag",     (const lua_CFunction)&_lv_obj_add_flag);
    lua.Lua_register("lv_obj_clear_flag",   (const lua_CFunction)&_lv_obj_clear_flag);

    lua.Lua_register("lv_scr_load",         (const lua_CFunction)&_lv_scr_load);
    lua.Lua_register("lv_scr_load_anim",    (const lua_CFunction)&_lv_scr_load_anim);

    lua.Lua_register("lv_obj_set_flex_flow",(const lua_CFunction)&_lv_obj_set_flex_flow);


    lua.Lua_register("lv_timer_handler",    (const lua_CFunction)&_lv_timer_handler);
    lua.Lua_register("lv_tick_inc",         (const lua_CFunction)&_lv_tick_inc);


    lua_State *L = lua.lua_state;
    auto reg_int = [&](const char *name, int v) {
        lua_pushinteger(L, v);
        lua_setglobal(L, name);
    };


    reg_int("LV_ALIGN_DEFAULT",         LV_ALIGN_DEFAULT);
    reg_int("LV_ALIGN_TOP_LEFT",        LV_ALIGN_TOP_LEFT);
    reg_int("LV_ALIGN_TOP_MID",         LV_ALIGN_TOP_MID);
    reg_int("LV_ALIGN_TOP_RIGHT",       LV_ALIGN_TOP_RIGHT);
    reg_int("LV_ALIGN_BOTTOM_LEFT",     LV_ALIGN_BOTTOM_LEFT);
    reg_int("LV_ALIGN_BOTTOM_MID",      LV_ALIGN_BOTTOM_MID);
    reg_int("LV_ALIGN_BOTTOM_RIGHT",    LV_ALIGN_BOTTOM_RIGHT);
    reg_int("LV_ALIGN_LEFT_MID",        LV_ALIGN_LEFT_MID);
    reg_int("LV_ALIGN_RIGHT_MID",       LV_ALIGN_RIGHT_MID);
    reg_int("LV_ALIGN_CENTER",          LV_ALIGN_CENTER);
    reg_int("LV_OBJ_FLAG_HIDDEN",       LV_OBJ_FLAG_HIDDEN);
    reg_int("LV_OBJ_FLAG_CLICKABLE",    LV_OBJ_FLAG_CLICKABLE);
    reg_int("LV_OBJ_FLAG_SCROLLABLE",   LV_OBJ_FLAG_SCROLLABLE);

 
    reg_int("LV_LABEL_LONG_WRAP",       LV_LABEL_LONG_WRAP);
    reg_int("LV_LABEL_LONG_DOT",        LV_LABEL_LONG_DOT);
    reg_int("LV_LABEL_LONG_SCROLL",     LV_LABEL_LONG_SCROLL);
    reg_int("LV_LABEL_LONG_CLIP",       LV_LABEL_LONG_CLIP);

    reg_int("LV_FLEX_FLOW_ROW",         LV_FLEX_FLOW_ROW);
    reg_int("LV_FLEX_FLOW_COLUMN",      LV_FLEX_FLOW_COLUMN);
    reg_int("LV_FLEX_FLOW_ROW_WRAP",    LV_FLEX_FLOW_ROW_WRAP);
    reg_int("LV_FLEX_FLOW_COLUMN_WRAP", LV_FLEX_FLOW_COLUMN_WRAP);

 
    reg_int("LV_SCR_LOAD_ANIM_NONE",         LV_SCR_LOAD_ANIM_NONE);
    reg_int("LV_SCR_LOAD_ANIM_OVER_LEFT",    LV_SCR_LOAD_ANIM_OVER_LEFT);
    reg_int("LV_SCR_LOAD_ANIM_OVER_RIGHT",   LV_SCR_LOAD_ANIM_OVER_RIGHT);
    reg_int("LV_SCR_LOAD_ANIM_OVER_TOP",     LV_SCR_LOAD_ANIM_OVER_TOP);
    reg_int("LV_SCR_LOAD_ANIM_OVER_BOTTOM",  LV_SCR_LOAD_ANIM_OVER_BOTTOM);
    reg_int("LV_SCR_LOAD_ANIM_MOVE_LEFT",    LV_SCR_LOAD_ANIM_MOVE_LEFT);
    reg_int("LV_SCR_LOAD_ANIM_MOVE_RIGHT",   LV_SCR_LOAD_ANIM_MOVE_RIGHT);
    reg_int("LV_SCR_LOAD_ANIM_MOVE_TOP",     LV_SCR_LOAD_ANIM_MOVE_TOP);
    reg_int("LV_SCR_LOAD_ANIM_MOVE_BOTTOM",  LV_SCR_LOAD_ANIM_MOVE_BOTTOM);
    reg_int("LV_SCR_LOAD_ANIM_FADE_ON",      LV_SCR_LOAD_ANIM_FADE_ON);


    reg_int("LV_DIR_TOP",    LV_DIR_TOP);
    reg_int("LV_DIR_BOTTOM", LV_DIR_BOTTOM);
    reg_int("LV_DIR_LEFT",   LV_DIR_LEFT);
    reg_int("LV_DIR_RIGHT",  LV_DIR_RIGHT);
}

#include "LVGLFs.h"
#include <lvgl.h>
#include <LittleFS.h>
#include <Arduino.h>


#define LV_FS_LETTER 'S'


static bool fs_ready_cb(lv_fs_drv_t *drv)
{
    return true;
}


static void *fs_open_cb(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode)
{
    const char *mode_str = "r";
    if (mode == LV_FS_MODE_WR)
        mode_str = "w";
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
        mode_str = "r+";

    File *f = new File(LittleFS.open(path, mode_str));
    if (!*f)
    {
        delete f;
        return NULL;
    }
    return (void *)f;
}


static lv_fs_res_t fs_close_cb(lv_fs_drv_t *drv, void *file_p)
{
    File *f = (File *)file_p;
    if (f)
    {
        f->close();
        delete f;
    }
    return LV_FS_RES_OK;
}


static lv_fs_res_t fs_read_cb(lv_fs_drv_t *drv, void *file_p,
                              void *buf, uint32_t btr, uint32_t *br)
{
    File *f = (File *)file_p;
    *br = f->read((uint8_t *)buf, btr);
    return LV_FS_RES_OK;
}

static lv_fs_res_t fs_seek_cb(lv_fs_drv_t *drv, void *file_p,
                              uint32_t pos, lv_fs_whence_t whence)
{
    File *f = (File *)file_p;
    SeekMode mode = SeekSet;
    if (whence == LV_FS_SEEK_CUR)
        mode = SeekCur;
    else if (whence == LV_FS_SEEK_END)
        mode = SeekEnd;
    return f->seek(pos, mode) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
}


static lv_fs_res_t fs_tell_cb(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p)
{
    File *f = (File *)file_p;
    *pos_p = f->position();
    return LV_FS_RES_OK;
}

static void *fs_dir_open_cb(lv_fs_drv_t *drv, const char *path)
{
    File *dir = new File(LittleFS.open(path));
    if (!*dir || !dir->isDirectory())
    {
        delete dir;
        return NULL;
    }
    return (void *)dir;
}

static lv_fs_res_t fs_dir_read_cb(lv_fs_drv_t *drv, void *rddir_p, char *fn)
{
    File *dir = (File *)rddir_p;
    File entry = dir->openNextFile();
    if (!entry)
    {
        fn[0] = '\0';
        return LV_FS_RES_OK;
    }
    String name = entry.name();
    if (entry.isDirectory())
        name = "/" + name;
    strncpy(fn, name.c_str(), LV_FS_MAX_PATH_LENGTH - 1);
    fn[LV_FS_MAX_PATH_LENGTH - 1] = '\0';
    return LV_FS_RES_OK;
}


static lv_fs_res_t fs_dir_close_cb(lv_fs_drv_t *drv, void *rddir_p)
{
    File *dir = (File *)rddir_p;
    if (dir)
    {
        dir->close();
        delete dir;
    }
    return LV_FS_RES_OK;
}


void lvgl_fs_init()
{
    static lv_fs_drv_t fs_drv;

    lv_fs_drv_init(&fs_drv);
    fs_drv.letter = LV_FS_LETTER;
    fs_drv.ready_cb = fs_ready_cb;
    fs_drv.open_cb = fs_open_cb;
    fs_drv.close_cb = fs_close_cb;
    fs_drv.read_cb = fs_read_cb;
    fs_drv.seek_cb = fs_seek_cb;
    fs_drv.tell_cb = fs_tell_cb;
    fs_drv.dir_open_cb = fs_dir_open_cb;
    fs_drv.dir_read_cb = fs_dir_read_cb;
    fs_drv.dir_close_cb = fs_dir_close_cb;

    lv_fs_drv_register(&fs_drv);

    Serial.println("[LVGL] LittleFS mounted as S:/");
}

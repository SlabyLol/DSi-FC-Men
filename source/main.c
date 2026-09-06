/*
 * DSi-FC-Men – Main Entry
 */
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "ui.h"
#include "splash.h"
#include "filebrowser.h"
#include "bootstrap.h"
#include "sound.h"

static bool running = true;

static void init_graphics(void) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
}

static void exit_graphics(void) {
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    romfsInit();
    fsInit();
    cfguInit();
    init_graphics();

    Config cfg;
    if (!config_load(&cfg, "/_nds/DSi-FC-Men/config.ini"))
        config_set_defaults(&cfg);

    sound_init(&cfg);

    if (cfg.show_splash) {
        sound_play(SOUND_SPLASH);
        splash_show(cfg.splash_duration);
    }

    sound_bgm_play();

    filebrowser_init(&cfg);

    while (aptMainLoop() && running) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) {
            running = false;
            break;
        }
        if (kDown & KEY_SELECT) {
            static bool bgm_on = true;
            bgm_on = !bgm_on;
            if (bgm_on) sound_bgm_play();
            else sound_bgm_stop();
        }

        filebrowser_update(&cfg, kDown);

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT),
                        C2D_Color32(0x0A, 0x0A, 0x12, 0xFF));
        ui_draw_top(&cfg);
        ui_draw_bottom(&cfg);
        C3D_FrameEnd(0);
    }

    sound_bgm_stop();
    sound_exit();
    config_free(&cfg);
    exit_graphics();
    cfguExit();
    fsExit();
    romfsExit();
    return 0;
}

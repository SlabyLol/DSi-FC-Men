/*
 * DSi-FC-Men – Single CIA
 * Auto-setup folders + config on launch. No separate setupper.
 */
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.h"
#include "ui.h"
#include "splash.h"
#include "filebrowser.h"
#include "bootstrap.h"
#include "sound.h"

#define VERSION "0.2.0"
#define SETUP_MARKER "/_nds/DSi-FC-Men/.setup_done"

static bool running = true;

static const char* FOLDERS[] = {
    "/_nds",
    "/_nds/DSi-FC-Men",
    "/_nds/DSi-FC-Men/themes",
    "/_nds/DSi-FC-Men/boxart",
    "/_nds/DSi-FC-Men/sounds",
    "/_nds/nds-bootstrap",
    "/roms",
    "/roms/nds",
    "/saves",
    NULL
};

static const char* DEFAULT_CONFIG =
"# DSi-FC-Men Configuration\n"
"Language = English\n"
"Theme = Default\n"
"ShowSplash = true\n"
"SplashDuration = 2\n"
"DefaultLauncher = nds-bootstrap\n"
"RomsPath = /roms/nds\n"
"SavesPath = /saves\n"
"EnableSounds = true\n"
"EnableBGM = true\n"
"UseDSiMode = true\n";

static bool file_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0;
}

static bool create_dir(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) return mkdir(path, 0777) == 0;
    return true;
}

static bool write_text(const char* path, const char* content) {
    FILE* f = fopen(path, "w");
    if (!f) return false;
    fputs(content, f);
    fclose(f);
    return true;
}

static void auto_setup(void) {
    for (int i = 0; FOLDERS[i]; i++)
        create_dir(FOLDERS[i]);

    if (!file_exists("/_nds/DSi-FC-Men/config.ini"))
        write_text("/_nds/DSi-FC-Men/config.ini", DEFAULT_CONFIG);

    write_text("/_nds/nds-bootstrap/README.txt",
        "Put nds-bootstrap files here.\n"
        "Download: https://github.com/DS-Homebrew/nds-bootstrap/releases/latest\n"
        "Need: nds-bootstrap-release.nds\n");

    write_text("/_nds/DSi-FC-Men/README.txt",
        "DSi-FC-Men " VERSION "\n"
        "ROMs -> /roms/nds/\n"
        "Sounds -> /_nds/DSi-FC-Men/sounds/\n");

    write_text(SETUP_MARKER, VERSION "\n");
}

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

    auto_setup();

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

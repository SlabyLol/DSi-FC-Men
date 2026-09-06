/*
 * DSi-FC-Men Setupper – 3DS CIA
 * Runs on the 3DS itself and prepares the microSD
 * for DSi-FC-Men in under a minute.
 */

#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define VERSION "0.1.0"

static C2D_TextBuf g_staticBuf;
static C2D_Text g_title, g_status, g_info;

static const char* FOLDERS[] = {
    "/_nds",
    "/_nds/DSi-FC-Men",
    "/_nds/DSi-FC-Men/themes",
    "/_nds/DSi-FC-Men/boxart",
    "/_nds/DSi-FC-Men/sounds",
    "/_nds/nds-bootstrap",
    "/roms",
    "/roms/nds",
    "/roms/gba",
    "/saves",
    "/themes",
    NULL
};

static const char* DEFAULT_CONFIG =
"# DSi-FC-Men Configuration\n"
"# Edit this file to change everything\n\n"
"[General]\n"
"Language = English\n"
"Theme = Default\n"
"ShowSplash = true\n"
"SplashDuration = 2\n"
"SoftReset = true\n"
"AutobootDelay = 0\n"
"ShowHiddenFiles = false\n\n"
"[Launcher]\n"
"DefaultLauncher = nds-bootstrap\n"
"ShowBoxart = true\n"
"ShowFileExtensions = false\n"
"SortBy = name\n"
"FoldersFirst = true\n\n"
"[Interface]\n"
"BackgroundColor = 0x0A0A12\n"
"TextColor = 0xFFFFFF\n"
"HighlightColor = 0x00B4FF\n"
"SecondaryColor = 0x1A1A2E\n"
"FontSize = 12\n"
"ShowBattery = true\n"
"ShowClock = true\n\n"
"[Paths]\n"
"RomsPath = /roms/nds\n"
"SavesPath = /saves\n"
"BoxartPath = /_nds/DSi-FC-Men/boxart\n"
"ThemesPath = /_nds/DSi-FC-Men/themes\n"
"SoundsPath = /_nds/DSi-FC-Men/sounds\n\n"
"[Advanced]\n"
"EnableLogging = false\n"
"FastBoot = false\n"
"UseDSiMode = true\n"
"EnableSounds = true\n";

bool create_dir(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        return mkdir(path, 0777) == 0;
    }
    return true;
}

bool write_file(const char* path, const char* content) {
    FILE* f = fopen(path, "w");
    if (!f) return false;
    fputs(content, f);
    fclose(f);
    return true;
}

void draw_text(C2D_Text* text, float x, float y, float scale, u32 color) {
    C2D_DrawText(text, C2D_WithColor, x, y, 0.5f, scale, scale, color);
}

int main(int argc, char** argv) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, NULL);

    g_staticBuf = C2D_TextBufNew(4096);
    C2D_TextParse(&g_title, g_staticBuf, "DSi-FC-Men Setupper");
    C2D_TextOptimize(&g_title);

    bool done = false;
    bool success = false;
    char status_msg[128] = "Press A to start setup";

    C2D_TextParse(&g_status, g_staticBuf, status_msg);
    C2D_TextOptimize(&g_status);

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break;

        if (!done && (kDown & KEY_A)) {
            strcpy(status_msg, "Creating folders...");
            C2D_TextBufClear(g_staticBuf);
            C2D_TextParse(&g_status, g_staticBuf, status_msg);
            C2D_TextOptimize(&g_status);

            bool ok = true;
            for (int i = 0; FOLDERS[i] != NULL; i++) {
                if (!create_dir(FOLDERS[i])) ok = false;
            }

            strcpy(status_msg, "Writing config.ini...");
            C2D_TextBufClear(g_staticBuf);
            C2D_TextParse(&g_status, g_staticBuf, status_msg);
            C2D_TextOptimize(&g_status);

            if (!write_file("/_nds/DSi-FC-Men/config.ini", DEFAULT_CONFIG)) ok = false;

            write_file("/_nds/DSi-FC-Men/README.txt",
                "DSi-FC-Men - DSi FlashCard Menu\n"
                "Put your .nds ROMs into /roms/nds/\n"
                "Edit config.ini to change everything.\n"
                "Sounds go into /_nds/DSi-FC-Men/sounds/\n");

            write_file("/_nds/nds-bootstrap/PLACE_NDS_BOOTSTRAP_HERE.txt",
                "Download latest nds-bootstrap from:\n"
                "https://github.com/DS-Homebrew/nds-bootstrap/releases\n"
                "and place the files here.\n");

            write_file("/_nds/DSi-FC-Men/sounds/README.txt",
                "Place sound files here (.bcwav / .wav)\n"
                "menu_move.bcwav\nmenu_select.bcwav\nmenu_back.bcwav\nsplash.bcwav\n");

            done = true;
            success = ok;
            if (ok) strcpy(status_msg, "Setup complete! Press START to exit");
            else strcpy(status_msg, "Setup finished with errors");
            C2D_TextBufClear(g_staticBuf);
            C2D_TextParse(&g_status, g_staticBuf, status_msg);
            C2D_TextOptimize(&g_status);
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT),
                        C2D_Color32(0x0A, 0x0A, 0x12, 0xFF));

        draw_text(&g_title, 20.0f, 30.0f, 0.9f, C2D_Color32(0x00, 0xB4, 0xFF, 0xFF));
        draw_text(&g_status, 20.0f, 100.0f, 0.6f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));

        printf("\x1b[2;1HDSi-FC-Men Setupper v%s", VERSION);
        printf("\x1b[4;1HThis CIA prepares your microSD");
        printf("\x1b[5;1Hfor DSi-FC-Men.");
        printf("\x1b[7;1HPress A = Start Setup");
        printf("\x1b[8;1HPress START = Exit");
        if (done) {
            printf("\x1b[10;1H%s", success ? "SUCCESS - Folders + config created" : "Finished with some errors");
            printf("\x1b[12;1HYou can now put ROMs into /roms/nds/");
        }

        C3D_FrameEnd(0);
    }

    C2D_TextBufDelete(g_staticBuf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}

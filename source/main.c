/*
 * DSi-FC-Men – Main Entry
 * DSi FlashCard Menu
 * Foundation version – citro2d based (3DS mode)
 * For pure NDS flashcard mode a libnds port is planned
 */

#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "config.h"
#include "ui.h"
#include "splash.h"
#include "filebrowser.h"

// Global state
C2D_SpriteSheet spriteSheet;
bool running = true;

void init_graphics(void) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    
    // Load sprite sheet later from SD
    // spriteSheet = C2D_SpriteSheetLoad("romfs:/sprites.t3x");
}

void exit_graphics(void) {
    // if (spriteSheet) C2D_SpriteSheetFree(spriteSheet);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}

int main(int argc, char* argv[]) {
    // Initialize services
    romfsInit();
    fsInit();
    cfguInit();
    
    init_graphics();
    
    // Load configuration from SD
    Config cfg;
    if (!config_load(&cfg, "/_nds/DSi-FC-Men/config.ini")) {
        // Fallback defaults
        config_set_defaults(&cfg);
    }
    
    // Show DSi-style splash
    if (cfg.show_splash) {
        splash_show(cfg.splash_duration);
    }
    
    // Main loop
    while (aptMainLoop() && running) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) {
            running = false;
            break;
        }
        
        // Begin frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT), 
                        C2D_Color32(0x0A, 0x0A, 0x12, 0xFF));
        
        // Draw UI
        ui_draw_top(&cfg);
        ui_draw_bottom(&cfg);
        
        // File browser logic
        filebrowser_update(&cfg, kDown);
        
        C3D_FrameEnd(0);
    }
    
    // Cleanup
    config_free(&cfg);
    exit_graphics();
    cfguExit();
    fsExit();
    romfsExit();
    
    return 0;
}

#include "splash.h"
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

/*
 * Simple DSi-style splash
 * In a full version this would load a real DSi boot logo / animation
 * from the SD card or romfs.
 */
void splash_show(int duration_seconds) {
    if (duration_seconds <= 0) return;

    C2D_TextBuf buf = C2D_TextBufNew(512);
    C2D_Text text;
    C2D_TextParse(&text, buf, "DSi-FC-Men");
    C2D_TextOptimize(&text);

    C2D_Text sub;
    C2D_TextParse(&sub, buf, "FlashCard Menu");
    C2D_TextOptimize(&sub);

    u64 start = osGetTime();
    while (aptMainLoop()) {
        u64 now = osGetTime();
        if ((now - start) / 1000 >= (u64)duration_seconds) break;

        hidScanInput();
        if (hidKeysDown() & (KEY_A | KEY_START | KEY_SELECT)) break;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        
        // Dark blue DSi-like background
        C2D_TargetClear(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT),
                        C2D_Color32(0x00, 0x33, 0x66, 0xFF));
        
        C2D_DrawText(&text, C2D_WithColor | C2D_AlignCenter,
                     200.0f, 90.0f, 0.5f, 1.2f, 1.2f,
                     C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));
        
        C2D_DrawText(&sub, C2D_WithColor | C2D_AlignCenter,
                     200.0f, 130.0f, 0.5f, 0.7f, 0.7f,
                     C2D_Color32(0xAA, 0xCC, 0xFF, 0xFF));

        C3D_FrameEnd(0);
    }

    C2D_TextBufDelete(buf);
}

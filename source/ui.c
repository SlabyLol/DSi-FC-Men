#include "ui.h"
#include <citro2d.h>
#include <stdio.h>
#include <time.h>

static C2D_TextBuf g_staticBuf;
static C2D_Text g_titleText;
static bool ui_initialized = false;

static void ui_init_once(void) {
    if (ui_initialized) return;
    g_staticBuf = C2D_TextBufNew(4096);
    C2D_TextParse(&g_titleText, g_staticBuf, "DSi-FC-Men");
    C2D_TextOptimize(&g_titleText);
    ui_initialized = true;
}

void ui_draw_top(const Config* cfg) {
    ui_init_once();

    // Background already cleared in main
    // Draw title
    C2D_DrawText(&g_titleText, C2D_WithColor, 20.0f, 20.0f, 0.5f, 1.0f, 1.0f,
                 C2D_Color32((cfg->highlight_color >> 16) & 0xFF,
                             (cfg->highlight_color >> 8) & 0xFF,
                             cfg->highlight_color & 0xFF, 255));

    // Simple status bar
    char status[128];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(status, sizeof(status), "%02d:%02d", t->tm_hour, t->tm_min);

    // For now just a colored bar
    C2D_DrawRectSolid(0, 220, 0.4f, 400, 20, 
                      C2D_Color32(0x1A, 0x1A, 0x2E, 220));
}

void ui_draw_bottom(const Config* cfg) {
    // Bottom screen will show file list later
    C2D_DrawRectSolid(0, 0, 0.3f, 320, 240,
                      C2D_Color32((cfg->bg_color >> 16) & 0xFF,
                                  (cfg->bg_color >> 8) & 0xFF,
                                  cfg->bg_color & 0xFF, 255));
}

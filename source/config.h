#ifndef DSI_FC_MEN_CONFIG_H
#define DSI_FC_MEN_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    // General
    char language[32];
    char theme[64];
    bool show_splash;
    int  splash_duration;
    bool soft_reset;
    int  autoboot_delay;
    bool show_hidden;

    // Launcher
    char default_launcher[32];   // "nds-bootstrap" or "kernel"
    bool show_boxart;
    bool show_extensions;
    char sort_by[16];
    bool folders_first;

    // Interface colors (RGB888 packed)
    uint32_t bg_color;
    uint32_t text_color;
    uint32_t highlight_color;
    uint32_t secondary_color;
    int      font_size;
    bool     show_battery;
    bool     show_clock;

    // Paths
    char roms_path[128];
    char saves_path[128];
    char boxart_path[128];
    char themes_path[128];

    // Advanced
    bool enable_logging;
    bool fast_boot;
    bool use_dsi_mode;
} Config;

// Load config from microSD ini file
bool config_load(Config* cfg, const char* path);

// Set safe defaults
void config_set_defaults(Config* cfg);

// Free any allocated resources (currently none)
void config_free(Config* cfg);

// Simple color parser (0xRRGGBB)
uint32_t parse_color(const char* str);

#endif

#include "bootstrap.h"
#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static bool file_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

bool bootstrap_available(void) {
    return file_exists(BOOTSTRAP_RELEASE_NDS) ||
           file_exists(BOOTSTRAP_HB_NDS) ||
           file_exists(BOOTSTRAP_NIGHTLY_NDS);
}

bool bootstrap_write_ini(const char* rom_path, const char* sav_path, const Config* cfg) {
    FILE* f = fopen(BOOTSTRAP_INI, "w");
    if (!f) return false;
    fprintf(f, "[NDS-BOOTSTRAP]\n");
    fprintf(f, "NDS_PATH = %s\n", rom_path ? rom_path : "");
    fprintf(f, "SAV_PATH = %s\n", sav_path ? sav_path : "");
    fprintf(f, "RAM_DRIVE_PATH = sd:/\n");
    fprintf(f, "GUI_LANGUAGE = en\n");
    fprintf(f, "CACHE_BLOCK_SIZE = 0\n");
    fprintf(f, "DEBUG = 0\n");
    fprintf(f, "LOGGING = 0\n");
    fprintf(f, "SOUND_FREQ = 1\n");
    fprintf(f, "LOADING_SCREEN = 1\n");
    fprintf(f, "LANGUAGE = -1\n");
    fprintf(f, "REGION = -1\n");
    fprintf(f, "DSI_MODE = %d\n", cfg && cfg->use_dsi_mode ? 1 : 0);
    fprintf(f, "BOOST_CPU = 1\n");
    fprintf(f, "BOOST_VRAM = 1\n");
    fprintf(f, "CARD_READ_DMA = 1\n");
    fclose(f);
    return true;
}

bool bootstrap_launch_rom(const char* rom_path, const Config* cfg) {
    if (!rom_path || !bootstrap_available()) return false;
    char sav_path[512];
    const char* base = strrchr(rom_path, '/');
    base = base ? base + 1 : rom_path;
    char name[256];
    strncpy(name, base, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    char* dot = strrchr(name, '.');
    if (dot) *dot = '\0';
    const char* saves = (cfg && cfg->saves_path[0]) ? cfg->saves_path : "/saves";
    snprintf(sav_path, sizeof(sav_path), "%s/%s.sav", saves, name);
    mkdir(saves, 0777);
    if (!bootstrap_write_ini(rom_path, sav_path, cfg)) return false;
    FILE* lp = fopen("/_nds/DSi-FC-Men/last_played.txt", "w");
    if (lp) { fprintf(lp, "%s\n%s\n", rom_path, sav_path); fclose(lp); }
    return true;
}

bool bootstrap_launch_hb(const char* nds_path, const Config* cfg) {
    if (!nds_path) return false;
    if (!file_exists(BOOTSTRAP_HB_NDS) && !bootstrap_available()) return false;
    return bootstrap_write_ini(nds_path, "", cfg);
}

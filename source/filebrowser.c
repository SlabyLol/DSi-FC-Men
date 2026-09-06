#include "filebrowser.h"
#include "bootstrap.h"
#include "sound.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENTRIES 256
#define MAX_NAME    256

typedef struct {
    char name[MAX_NAME];
    bool is_dir;
} DirEntry;

static DirEntry entries[MAX_ENTRIES];
static int entry_count = 0;
static int selected = 0;
static char current_path[512] = "/roms/nds";
static bool initialized = false;

static int compare_entries(const void* a, const void* b) {
    const DirEntry* ea = a;
    const DirEntry* eb = b;
    if (ea->is_dir != eb->is_dir) return ea->is_dir ? -1 : 1;
    return strcasecmp(ea->name, eb->name);
}

static void scan_dir(const Config* cfg) {
    entry_count = 0;
    DIR* dir = opendir(current_path);
    if (!dir) {
        strcpy(current_path, "/");
        dir = opendir(current_path);
    }
    if (!dir) return;
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL && entry_count < MAX_ENTRIES) {
        if (strcmp(ent->d_name, ".") == 0) continue;
        if (!cfg->show_hidden && ent->d_name[0] == '.') continue;
        strncpy(entries[entry_count].name, ent->d_name, MAX_NAME - 1);
        entries[entry_count].is_dir = (ent->d_type == DT_DIR);
        entry_count++;
    }
    closedir(dir);
    qsort(entries, entry_count, sizeof(DirEntry), compare_entries);
    selected = 0;
}

void filebrowser_init(const Config* cfg) {
    if (initialized) return;
    strncpy(current_path, cfg->roms_path, sizeof(current_path) - 1);
    scan_dir(cfg);
    initialized = true;
}

void filebrowser_update(const Config* cfg, u32 kDown) {
    if (!initialized) filebrowser_init(cfg);
    if (entry_count == 0) return;

    if (kDown & KEY_UP) {
        selected--;
        if (selected < 0) selected = entry_count - 1;
        sound_play(SOUND_MOVE);
    }
    if (kDown & KEY_DOWN) {
        selected++;
        if (selected >= entry_count) selected = 0;
        sound_play(SOUND_MOVE);
    }

    if (kDown & KEY_A) {
        DirEntry* e = &entries[selected];
        if (e->is_dir) {
            char newpath[512];
            if (strcmp(current_path, "/") == 0)
                snprintf(newpath, sizeof(newpath), "/%s", e->name);
            else
                snprintf(newpath, sizeof(newpath), "%s/%s", current_path, e->name);
            strncpy(current_path, newpath, sizeof(current_path) - 1);
            scan_dir(cfg);
            sound_play(SOUND_SELECT);
        } else {
            char full[512];
            if (strcmp(current_path, "/") == 0)
                snprintf(full, sizeof(full), "/%s", e->name);
            else
                snprintf(full, sizeof(full), "%s/%s", current_path, e->name);
            size_t len = strlen(e->name);
            sound_play(SOUND_SELECT);
            if (len > 4 && strcasecmp(e->name + len - 4, ".nds") == 0) {
                if (bootstrap_available())
                    bootstrap_launch_rom(full, cfg);
            }
        }
    }

    if (kDown & KEY_B) {
        sound_play(SOUND_BACK);
        char* slash = strrchr(current_path, '/');
        if (slash && slash != current_path) *slash = '\0';
        else strcpy(current_path, "/");
        scan_dir(cfg);
    }
}

void filebrowser_draw(const Config* cfg) {
    (void)cfg;
}

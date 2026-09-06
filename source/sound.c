#include "sound.h"
#include <3ds.h>
#include <stdio.h>
#include <string.h>

static bool sounds_enabled = true;
static bool initialized = false;

bool sound_init(const Config* cfg) {
    if (initialized) return true;
    
    // Will later read cfg->enable_sounds and load from cfg->sounds_path
    sounds_enabled = true;
    
    // TODO: ndspInit() + load bcwav/wav files
    initialized = true;
    return true;
}

void sound_exit(void) {
    if (!initialized) return;
    // TODO: free samples, ndspExit()
    initialized = false;
}

void sound_play(SoundId id) {
    if (!initialized || !sounds_enabled) return;
    
    switch (id) {
        case SOUND_MOVE:   break;
        case SOUND_SELECT: break;
        case SOUND_BACK:   break;
        case SOUND_SPLASH: break;
        default: break;
    }
}

void sound_set_enabled(bool enabled) {
    sounds_enabled = enabled;
}

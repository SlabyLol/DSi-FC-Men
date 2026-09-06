#ifndef DSI_FC_MEN_SOUND_H
#define DSI_FC_MEN_SOUND_H

#include <stdbool.h>
#include "config.h"

typedef enum {
    SOUND_MOVE = 0,
    SOUND_SELECT,
    SOUND_BACK,
    SOUND_SPLASH,
    SOUND_COUNT
} SoundId;

bool sound_init(const Config* cfg);
void sound_exit(void);
void sound_play(SoundId id);
void sound_set_enabled(bool enabled);

/* Looping menu background music */
void sound_bgm_play(void);
void sound_bgm_stop(void);
void sound_bgm_set_enabled(bool enabled);

#endif

#ifndef DSI_FC_MEN_SETUP_H
#define DSI_FC_MEN_SETUP_H

#include <3ds.h>
#include <stdbool.h>
#include "config.h"

typedef enum {
    MODE_NONE = 0,
    MODE_FLASHCARD,
    MODE_DEVICE_SD,
    MODE_BOTH
} BootMode;

typedef enum {
    SETUP_PHASE_SELECT = 0,
    SETUP_PHASE_CONFIGURE,
    SETUP_PHASE_CHOOSE_BOOT,
    SETUP_PHASE_FC_MISSING,
    SETUP_PHASE_DONE
} SetupPhase;

typedef struct {
    SetupPhase phase;
    bool want_flashcard;
    bool want_device_sd;
    bool flashcard_present;
    bool cursor_on_install;
    int  select_cursor;
    int  boot_cursor;
} SetupState;

void setup_init(SetupState* s);
void setup_update(SetupState* s, u32 kDown);
void setup_draw(const SetupState* s);
bool setup_finished(const SetupState* s);
BootMode setup_get_mode(const SetupState* s);
bool setup_detect_flashcard(void);

#endif

#include "input.h"
#include <string.h>

#define STICK_NAV  40
#define REPEAT_FIRST 18
#define REPEAT_NEXT  6

static int rep_up = 0, rep_down = 0, rep_left = 0, rep_right = 0;

void input_init(void) {
    rep_up = rep_down = rep_left = rep_right = 0;
}

static bool axis_held_repeat(bool active, int* timer) {
    if (!active) { *timer = 0; return false; }
    if (*timer == 0) { *timer = 1; return true; }
    (*timer)++;
    if (*timer == REPEAT_FIRST) return true;
    if (*timer > REPEAT_FIRST && ((*timer - REPEAT_FIRST) % REPEAT_NEXT) == 0) return true;
    return false;
}

void input_update(Input* in) {
    memset(in, 0, sizeof(*in));
    hidScanInput();
    in->down = hidKeysDown();
    in->held = hidKeysHeld();
    in->up   = hidKeysUp();

    circlePosition pos;
    hidCircleRead(&pos);
    in->stick_x = pos.dx / 156.0f;
    in->stick_y = pos.dy / 156.0f;
    in->stick_up    = pos.dy > STICK_NAV;
    in->stick_down  = pos.dy < -STICK_NAV;
    in->stick_left  = pos.dx < -STICK_NAV;
    in->stick_right = pos.dx > STICK_NAV;

    circlePosition cpos;
    hidCstickRead(&cpos);
    in->cstick_x = cpos.dx / 156.0f;
    in->cstick_y = cpos.dy / 156.0f;
    in->cstick_up    = cpos.dy > STICK_NAV;
    in->cstick_down  = cpos.dy < -STICK_NAV;
    in->cstick_left  = cpos.dx < -STICK_NAV;
    in->cstick_right = cpos.dx > STICK_NAV;

    touchPosition touch;
    hidTouchRead(&touch);
    in->touch = (in->held & KEY_TOUCH) != 0;
    in->touch_down = (in->down & KEY_TOUCH) != 0;
    in->touch_up = (in->up & KEY_TOUCH) != 0;
    in->touch_x = touch.px;
    in->touch_y = touch.py;

    bool raw_up = (in->held & KEY_UP) || in->stick_up || in->cstick_up;
    bool raw_down = (in->held & KEY_DOWN) || in->stick_down || in->cstick_down;
    bool raw_left = (in->held & KEY_LEFT) || in->stick_left || in->cstick_left;
    bool raw_right = (in->held & KEY_RIGHT) || in->stick_right || in->cstick_right;

    if (in->down & KEY_UP) { in->nav_up = true; rep_up = 1; }
    else in->nav_up = axis_held_repeat(raw_up, &rep_up);

    if (in->down & KEY_DOWN) { in->nav_down = true; rep_down = 1; }
    else in->nav_down = axis_held_repeat(raw_down, &rep_down);

    if (in->down & KEY_LEFT) { in->nav_left = true; rep_left = 1; }
    else in->nav_left = axis_held_repeat(raw_left, &rep_left);

    if (in->down & KEY_RIGHT) { in->nav_right = true; rep_right = 1; }
    else in->nav_right = axis_held_repeat(raw_right, &rep_right);

    in->confirm = (in->down & KEY_A) || in->touch_down;
    in->cancel  = (in->down & KEY_B) != 0;
    in->extra   = (in->down & KEY_X) != 0;
    in->option  = (in->down & KEY_Y) != 0;
    in->menu    = (in->down & KEY_START) != 0;
    in->select  = (in->down & KEY_SELECT) != 0;
    in->shoulder_l = (in->down & KEY_L) != 0;
    in->shoulder_r = (in->down & KEY_R) != 0;
    in->zl = (in->down & KEY_ZL) != 0;
    in->zr = (in->down & KEY_ZR) != 0;
}

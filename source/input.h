#ifndef DSI_FC_MEN_INPUT_H
#define DSI_FC_MEN_INPUT_H

#include <3ds.h>
#include <stdbool.h>

typedef struct {
    u32 down;
    u32 held;
    u32 up;
    float stick_x;
    float stick_y;
    bool stick_up;
    bool stick_down;
    bool stick_left;
    bool stick_right;
    float cstick_x;
    float cstick_y;
    bool cstick_up;
    bool cstick_down;
    bool cstick_left;
    bool cstick_right;
    bool touch;
    bool touch_down;
    bool touch_up;
    s16 touch_x;
    s16 touch_y;
    bool nav_up;
    bool nav_down;
    bool nav_left;
    bool nav_right;
    bool confirm;
    bool cancel;
    bool extra;
    bool option;
    bool menu;
    bool select;
    bool shoulder_l;
    bool shoulder_r;
    bool zl;
    bool zr;
} Input;

void input_init(void);
void input_update(Input* in);

#endif

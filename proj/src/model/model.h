#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <lcom/lcf.h>

#include "controller/timer/timer.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/graphics/graphics.h"
#include "view/view.h"

#include "sprites.h"
#include "xpm/xpm_mouse.h"

/* Interactable Objects */
Sprite* mouse;

typedef enum {
    MAIN_MENU,
    GAME,
    STOP,
} MainStateMachine;

void (timer_int)();

void (keyboard_int)();

void (mouse_int)();

void (parse_mouse_data)();

void (mouse_update)();

/* Sprite Creation */

void (loader_sprite)();

void (unloader_sprite)();

#endif

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
#include "xpm/play_unselected.h"
#include "xpm/play_selected.h"
#include "xpm/leaderboard_unselected.h"
#include "xpm/leaderboard_selected.h"
#include "xpm/exit_unselected.h"
#include "xpm/exit_selected.h"
#include "xpm/blue_puck.h"
#include "xpm/red_puck.h"


/* Interactable Objects */
Sprite* mouse;
Sprite* menu_button;
Sprite* menu_2button;
Sprite* menu_3button;
Sprite* menu_4button;
Sprite* menu_5button;
Sprite* menu_6button;
Sprite* redpuck;
Sprite* bluepuck;


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

void (option_up)();

void (option_down)();

#endif

#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <lcom/lcf.h>

#include "controller/timer/timer.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/graphics/graphics.h"
#include "view/view.h"

#include "physics.h"
#include "sprites.h"
#include "xpm/xpm_mouse.h"
#include "xpm/play_unselected.h"
#include "xpm/play_selected.h"
#include "xpm/leaderboard_unselected.h"
#include "xpm/leaderboard_selected.h"
#include "xpm/exit_unselected.h"
#include "xpm/exit_selected.h"
#include "xpm/play_again_selected.h"
#include "xpm/play_again_unselected.h"
#include "xpm/leave_selected.h"
#include "xpm/leave_unselected.h"
#include "xpm/blue_puck.h"
#include "xpm/red_puck.h"
#include "xpm/disk.h"

/* Interactable Objects */
Sprite* mouse;
Sprite* redpuck;
Sprite* bluepuck;
Sprite* Ball;

/* Menu buttons */
Sprite* exit_button_unselected;
Sprite* leaderboard_button_unselected;
Sprite* start_button_unselected;
Sprite* buttons_menu_unselected[3];

Sprite* start_button_selected;
Sprite* leaderboard_button_selected;
Sprite* exit_button_selected;
Sprite* buttons_menu_selected[3];

/* Win-Lose buttons */
Sprite* play_again_button_selected;
Sprite* leave_button_selected;
Sprite* buttons_winlose_selected[2];

Sprite* play_again_button_unselected;
Sprite* leave_button_unselected;
Sprite* buttons_winlose_unselected[2];


typedef enum {
    MAIN_MENU,
    GAME,
    STOP,
    WIN,
    LOST,
} MainStateMachine;

void reset_option();

void (timer_int)();

void (keyboard_int)();

void (mouse_int)();

void (parse_mouse_data)();

void (mouse_update)();

/* Sprite Creation */

void (loader_sprite)();

void (unloader_sprite)();

void (option_up)(Sprite* buttons_unselected[], Sprite* buttons_selected[], int size);

void (option_down)(Sprite* buttons_unselected[], Sprite* buttons_selected[], int size);

#endif

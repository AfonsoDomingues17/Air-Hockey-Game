#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <lcom/lcf.h>

#include "controller/timer/timer.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/graphics/graphics.h"
#include "controller/rtc/rtc.h"
#include "controller/serialport/serialport.h"
#include "view/view.h"

#include "physics.h"
#include "sprites.h"

#include "xpm/xpm_mouse.h"
#include "xpm/play_unselected.h"
#include "xpm/play_selected.h"
#include "xpm/rules_unselected.h"
#include "xpm/rules_selected.h"
#include "xpm/exit_unselected.h"
#include "xpm/exit_selected.h"
#include "xpm/play_again_selected.h"
#include "xpm/play_again_unselected.h"
#include "xpm/leave_selected.h"
#include "xpm/leave_unselected.h"
#include "xpm/blue_puck.h"
#include "xpm/red_puck.h"
#include "xpm/disk.h"
#include "xpm/game/white numbers/white_0.h"
#include "xpm/game/white numbers/white_1.h"
#include "xpm/game/white numbers/white_2.h"
#include "xpm/game/white numbers/white_3.h"
#include "xpm/game/white numbers/white_4.h"
#include "xpm/game/white numbers/white_5.h"
#include "xpm/game/white numbers/white_6.h"
#include "xpm/game/white numbers/white_7.h"
#include "xpm/game/white numbers/white_8.h"
#include "xpm/game/white numbers/white_9.h"
#include "xpm/game/white numbers/white_time.h"
#include "xpm/game/time.h"
#include "xpm/game/points.h"
#include "xpm/game/white numbers/white_points.h"
#include "xpm/game/red numbers/red_0.h"
#include "xpm/game/red numbers/red_1.h"
#include "xpm/game/red numbers/red_2.h"
#include "xpm/game/red numbers/red_3.h"
#include "xpm/game/red numbers/red_4.h"
#include "xpm/game/red numbers/red_5.h"
#include "xpm/game/red numbers/red_6.h"
#include "xpm/game/red numbers/red_7.h"
#include "xpm/game/red numbers/red_8.h"
#include "xpm/game/red numbers/red_9.h"
#include "xpm/game/blue numbers/blue_0.h"
#include "xpm/game/blue numbers/blue_1.h"
#include "xpm/game/blue numbers/blue_2.h"
#include "xpm/game/blue numbers/blue_3.h"
#include "xpm/game/blue numbers/blue_4.h"
#include "xpm/game/blue numbers/blue_5.h"
#include "xpm/game/blue numbers/blue_6.h"
#include "xpm/game/blue numbers/blue_7.h"
#include "xpm/game/blue numbers/blue_8.h"
#include "xpm/game/blue numbers/blue_9.h"
#include "xpm/day_separator.h"
#include "xpm/time_two_points.h"


/* Interactable Objects */
Sprite* mouse;
Sprite* redpuck;
Sprite* bluepuck;
Sprite* Ball;
Sprite *numbers[10];
Sprite *time_sep;
Sprite *time_title;
Sprite *points_title;
Sprite *numbers_red[10];
Sprite *numbers_blue[10];
Sprite *points_sep;
Sprite *two_points;
Sprite *two_points2;
Sprite *day_sep;
Sprite *day_sep2;

/* Menu buttons */
Sprite* exit_button_unselected;
Sprite* rules_button_unselected;
Sprite* start_button_unselected;
Sprite* buttons_menu_unselected[3];

Sprite* start_button_selected;
Sprite* rules_button_selected;
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
    RULES,
    GAME,
    STOP,
    WIN,
    LOST,
    TIE,
} MainStateMachine;

/* Menu Related */

/**
 * @brief Function to handle the actions of the buttons in the states win, lost, and tie.
*/
void popOutButtons();

/* Interrupt handling */

/**
 * @brief Function called when the driver receives a timer interrupt
 * @see timer_int_handler
*/
void (timer_int)();

/**
 * @brief Function called when the driver receives a keyboard interrupt
 * @see kbc_ih
*/
void (keyboard_int)();

/**
 * @brief Function called when the driver receives a mouse interrupt
 * @see mouse_ih
*/
void (mouse_int)();

/**
 * @brief Function called when the driver receives a serial line interrupt
 * @see sp_ih
*/
void (sp_int)();

/**
 * @brief Extracts information of the mouse to a packet struct
 * @param parsing Parsed packet
*/
void (parse_mouse_data)(struct packet* parsing);

/**
 * @brief Update the position of the mouse sprite using parsed x and y information.
 * @param mouse Pointer to the mouse sprite.
 * @param parsing Parsed packet containing x and y coordinates.
*/
void (mouse_update)(Sprite* mouse, struct packet parsing);

/**
 * @brief Function called when the driver receives a real-timer clock interrupt
 * @see timer_int_handler
*/
void (rtc_int)();

/* Auxiliary Functions */

void (reset_game_state)();

/* Sprite Creation */

/**
 * @brief Load predefined sprites.
*/
void (loader_sprite)();

/**
 * @brief Delete all sprites loaded
*/
void (unloader_sprite)();

/**
 * @brief When the 'W' key is pressed, change the currently selected/unselected button to false and the next button to true
 * 
 * @param buttons_unselected Array containing the unselected sprite buttons.
 * @param buttons_selected Array containing the selected sprite buttons.
*/
void (option_up)(Sprite* buttons_unselected[], Sprite* buttons_selected[], int size);

/**
 * @brief When the 'S' key is pressed, change the currently selected/unselected button to false and the next button to true
 * 
 * @param buttons_unselected Array containing the unselected sprite buttons.
 * @param buttons_selected Array containing the selected sprite buttons.
*/
void (option_down)(Sprite* buttons_unselected[], Sprite* buttons_selected[], int size);

#endif

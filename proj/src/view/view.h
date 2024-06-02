#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_

#include <lcom/lcf.h>

#include "controller/graphics/graphics.h"
#include "model/model.h"

/* Static XPMS */
#include "model/xpm/xpm_background.h"
#include "model/xpm/menu.h"
#include "model/xpm/youwon.h"
#include "model/xpm/youlost.h"
#include "model/xpm/tie.h"
#include "model/xpm/rules.h"

/**
 * @brief Draws the current frame based on the state of the main state.
 */
void (draw_frame)();

/**
 * @brief Swap the contents of the main buffer with the secondary buffer.
 */
void(swap_buffers)();

/**
 * @brief Draws a given .xpm file as the background image.
 * 
 * @param xpm The .xpm file to be drawn.
 * @param img Pointer to the image holder of the .xpm file.
 * @return 0 if sucessful, 1 otherwise
 */
int (vg_draw_background)(xpm_map_t xpm, xpm_image_t *img);

/**
 * @brief Draws a given .xpm file in the middle of the screen
 * 
 * @param xpm The .xpm file to be drawn.
 * @param img Pointer to the image holder of the .xpm file.
 * @return 0 if sucessful, 1 otherwise
 */
int (vg_drawn_popOut)(xpm_map_t xpm, xpm_image_t *img);

/**
 * @brief Draws the specified time at the given coordinates
 * 
 * @param time The desired time to be drawn
 * @param x The x-coordinate of the starting point
 * @param y The y-coordinate of the starting point
 */
void (vg_draw_time)(unsigned int time, int x, int y);

/**
 * @brief Draws the specified time at the given coordinates
 * 
 * @param player_1 Player 1 pontuation ot be drawn
 * @param player_2 Player 2 pontuation ot be drawn
 * @param x The x-coordinate of the starting point
 * @param y The y-coordinate of the starting point
 */
void (vg_draw_points)(int player_1, int player_2, int x, int y);

/**
 * @brief Draws the real-timer clock time at the given coordinates
 * 
 * @param time The struct that contains the information of the real-timer clock
 * @param x The x-coordinate of the starting point
 * @param y The y-coordinate of the starting point
*/
void (vg_draw_rtc)(day_time *time, int x, int y);

#endif

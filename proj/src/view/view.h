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

void (draw_frame)();

void(swap_buffers)();

int (vg_draw_background)(xpm_map_t xpm, xpm_image_t *img);

int (vg_drawn_popOut)(xpm_map_t xpm, xpm_image_t *img);

void (vg_draw_time)(unsigned int time, int x, int y);

void (vg_draw_points)(int player_1, int player_2, int x, int y);

void (vg_draw_rtc)(day_time *time, int x, int y);

#endif

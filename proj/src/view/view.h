#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_

#include <lcom/lcf.h>

#include "controller/graphics/graphics.h"
#include "model/model.h"

/* Static XPMS */
#include "model/xpm/xpm_background.h"
#include "model/xpm/menu.h"
#include "model/xpm/red_puck.h"
#include "model/xpm/blue_puck.h"
#include "model/xpm/disk.h"

/* Sprite Objects */
extern Sprite* mouse;
extern Sprite* menu_button;
extern Sprite* menu_2button;
extern Sprite* menu_3button;

void (draw_frame)();

void(swap_buffers)();

int (vg_draw_background)(xpm_map_t xpm, xpm_image_t *img);

int (draw_puck)(int x, int y, int player, xpm_image_t *puck);

#endif

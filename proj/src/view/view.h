#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_

#include <lcom/lcf.h>

#include "controller/graphics/graphics.h"
#include "model/model.h"

/* Static XPMS */
#include "model/xpm/xpm_background.h"
#include "model/xpm/menu.h"

/* Sprite Objects */
extern Sprite* mouse;
extern Sprite* menu_button;
extern Sprite* menu_2button;
extern Sprite* menu_3button;


void (draw_frame)();

void(swap_buffers)();

int (vg_draw_background)(xpm_map_t xpm, xpm_image_t *img);


#endif

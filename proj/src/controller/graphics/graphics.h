#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>

#include "../../model/sprites.h"

#define TRANSPARENT 0xFFFFFE

/* VBE */
#define VBE_CALL 0x4F00
#define SET_VBE_MODE 0x02
#define VBE_INT 0x10
#define INDEXED_MODEL 0x04

/* BIOS */
#define VIDEO_CARD_INT 0x10
#define SET_VIDEO_MODE 0x00
#define TEXT_MODE 0x03

#define SET_LINEAR_BIT BIT(14)

int (graphics_set_mode)(uint16_t mode);

int (map_phys_mem)(uint16_t mode);

void*(vg_init)(uint16_t mode);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

uint32_t (create_mask)(unsigned int size, unsigned int lsb);

uint32_t (get_color_component)(uint32_t color, uint32_t mask_size, uint32_t lsb);

int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);


int (vg_draw_sprite)(Sprite *sprite);

unsigned (get_h_res)();

unsigned (get_v_res)();

#endif

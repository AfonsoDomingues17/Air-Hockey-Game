#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>

/* VBE */
#define VBE_CALL 0x4F00
#define SET_VBE_MODE 0x02
#define VBE_INT 0x10
#define INDEXED_MODEL 0x04
#define INDEXED_MODE 0x105

/* BIOS */
#define VIDEO_CARD_INT 0x10
#define SET_VIDEO_MODE 0x00
#define TEXT_MODE 0x03

#define SET_LINEAR_BIT BIT(14)

int (set_graphic_mode)(uint16_t mode);

int (mset_frame_buffer)(uint16_t mode);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (read_outbuffer)(uint8_t *value);

uint32_t Red(uint32_t first);

uint32_t Green(uint32_t first);

uint32_t Blue(uint32_t first);

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

int util_sys_inb(int port, uint8_t* value);

#endif /* _LCOM_GRAPHICS_H */

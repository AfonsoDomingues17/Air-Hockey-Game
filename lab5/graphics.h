#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>
#include <machine/int86.h>

/* VBE */
#define VBE_CALL 0x4F00
#define SET_VBE_MODE 0x02
#define VBE_INT 0x10

/* BIOS */
#define VIDEO_CARD_INT 0x10
#define SET_VIDEO_MODE 0x00
#define TEXT_MODE 0x03

#define SET_LINEAR_BIT BIT(14)

int (graphics_set_mode)(uint16_t mode);

#endif

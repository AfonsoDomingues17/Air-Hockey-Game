#ifndef GRAPHICS_H
#define GRAPHICS_H

int set_VBE_mode(uint16_t mode); //SET MINIX 3 TO GRAPHIC MODE
void *(vg_init)(uint16_t mode);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int wait_for_esc();
int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);
int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif

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

/**
 * @brief Sets a given graphic mode
 * 
 * @param mode Contains the resolution method
 * @return 0 if sucessful, 1 otherwise
*/
int (graphics_set_mode)(uint16_t mode);

/**
 * @brief Perform the mapping of physical memory for video access
 * 
 * @param mode Contains the resolution method
 * @return 0 if sucessful, 1 otherwise
*/
int (map_phys_mem)(uint16_t mode);

/**
 * @brief Initialize static global variables, initialize graphical mode and perform memory mapping.
 * 
 * @param mode Contains the resolution method
 * @return main buffer
*/
void*(vg_init)(uint16_t mode);

/**
 * @brief Draws a horizontal line on the screen starting from the specified coordinates.
 * 
 * @param x The x-coordinate of the starting point of the line.
 * @param y The y-coordinate of the starting point of the line.
 * @param len The length of the line to be drawn.
 * @param color The color of the line.
 * @return 0 if sucessful, 1 otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Returns a bitmask indicating the position of a red, green, or blue component within a pixel.
 * 
 * @param size The size of the mask in bits.
 * @param lsb The least significant bit position of the mask.
 * @return The bitmask representing the position of the component within a pixel.
*/
uint32_t (create_mask)(unsigned int size, unsigned int lsb);

/**
 * @brief Retrieves a color component from a given color value using a specified bitmask.
 * 
 * @param color The color value from which to extract the component.
 * @param mask_size The size of the bitmask indicating the component's position within the color.
 * @param lsb The least significant bit position of the mask.
 * @return The extracted color component.
 */
uint32_t (get_color_component)(uint32_t color, uint32_t mask_size, uint32_t lsb);

/**
 * @brief Draws an XPM image at the specified coordinates.
 * 
 * @param xpm The XPM image to be drawn.
 * @param x The x-coordinate of the starting point of the image.
 * @param y The y-coordinate of the starting point of the image.
 * @return 0 if sucessful, 1 otherwise
 */
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Draws an Sprite object
 * 
 * @param sprite The sprite object containing the pixel map, height, and width.
 * @return 0 if sucessful, 1 otherwise
 */
int (vg_draw_sprite)(Sprite *sprite);

/**
 * @brief Gets horizontal resolution of the screen
 * 
 * @return horizontal resolution
*/
unsigned (get_h_res)();

/**
 * @brief Gets vertical resolution of the screen
 * 
 * @return Vertical resolution
*/
unsigned (get_v_res)();

#endif

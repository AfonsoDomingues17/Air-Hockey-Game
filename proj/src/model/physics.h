#ifndef _LCOM_PHYSICS_H_
#define _LCOM_PHYSICS_H_

#include <lcom/lcf.h>
#include "sprites.h"

/* Play area collision info */
#define horizontal_start 273
#define horizontal_end 879
#define vertical_start 21
#define vertical_end 843

/**
 * @brief Move a sprite object
 * @param sprite Sprite pointer to the sprite object
 * @param x next sprite location
 * @param y next sprite location
*/
void move(Sprite* object, int16_t x, int16_t y);

#endif

#ifndef _LCOM_SPRITES_H_
#define _LCOM_SPRITES_H_

#include <lcom/lcf.h>

typedef struct {
    int x, y; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    char *map; // the pixmap
} Sprite;


/**
 * @brief Create a sprite object
 * @param xpm XPM Map of the sprite object
 * @return Sprite* pointer to its object
*/
Sprite *create_sprite(xpm_map_t xpm);

/**
 * @brief Delete a sprite object
 * @param sprite Sprite pointer to the sprite object
*/
void *delete_sprite(Sprite *sp);

#endif

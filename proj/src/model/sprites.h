#ifndef _LCOM_SPRITES_H_
#define _LCOM_SPRITES_H_

#include <lcom/lcf.h>

typedef struct {
    int x, y; // current position
    uint16_t width, height; // dimensions
    int xspeed, yspeed; // current speed
    uint8_t *map; // the pixmap
    bool selected;
    bool visibility;
} Sprite;


/**
 * @brief Create a sprite object
 * @param xpm XPM Map of the sprite object
 * @param x Position x of the sprite on the screen
 * @param y Position y of the sprite on the screen
 * @return Sprite* pointer to its object
*/
Sprite *create_sprite(xpm_map_t xpm, int x, int y);

/**
 * @brief Delete a sprite object
 * @param sprite Sprite pointer to the sprite object
*/
void *delete_sprite(Sprite *sp);

#endif

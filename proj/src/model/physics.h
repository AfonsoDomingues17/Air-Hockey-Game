#ifndef _LCOM_PHYSICS_H_
#define _LCOM_PHYSICS_H_

#include <lcom/lcf.h>
#include "sprites.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/* Play area collision info */
#define horizontal_start 273
#define horizontal_end 879
#define vertical_start 21
#define vertical_end 843

/* Collidable objects */
extern Sprite* redpuck;
extern Sprite* bluepuck;
extern Sprite* Ball;

/**
 * @brief Move a sprite object
 * @param sprite Sprite pointer to the sprite object
 * @param x next sprite location
 * @param y next sprite location
*/
void move(Sprite* object, int16_t x, int16_t y, unsigned layer);

/**
 * @brief Detects if object collided with other objects on its layer
 * @param object Sprite pointer to the sprite object
 * @param new_x Top left corner x position that the object is trying to move into
 * @param new_y Top left corner y position that the object is trying to move into
 * @param layer Layer in which I want to detect collision
*/
bool detect_collision_in_layer(Sprite* object, int new_x, int new_y, unsigned layer);

/**
 * @brief Checks collision between two objects
 * @param object1 Sprite pointer to the sprite object1
 * @param new_x Top left corner x position that the object is trying to move into
 * @param new_y Top left corner y position that the object is trying to move into
 * @param object2 Sprite pointer to the sprite object2
*/
bool detec_collision(Sprite* object1, int new_x, int new_y, Sprite* object2);

#endif

#ifndef _LCOM_PHYSICS_H_
#define _LCOM_PHYSICS_H_

#include <lcom/lcf.h>
#include "sprites.h"
#include "controller/serialport/serialport.h"
#include "controller/graphics/graphics.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/* Play area collision info */
#define horizontal_start 273
#define horizontal_end 879
#define vertical_start 21
#define vertical_end 843
#define middle_field 432

/* Ball Specific Limitations */
#define max_velocity 16

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
bool detect_collision(Sprite* object1, int new_x, int new_y, Sprite* object2);

/**
 * @brief Returns true if the pixels inside a specific area overlapped between 2 objects
 * @param object1 Sprite pointer to the sprite object1
 * @param new_x Top left corner x position that the object is trying to move into
 * @param new_y Top left corner y position that the object is trying to move into
 * @param object2 Sprite pointer to the sprite object2
 * @param left_border Left most border of the overlapped region
 * @param right_border Right most border of the overlapped region
 * @param top_border Top most border of the overlapped region
 * @param bottom_border Bottom most border of the overlapped region
*/
bool pixel_detection(Sprite* object1, int new_x, int new_y, Sprite* object2, int left_border, int right_border, int top_border, int bottom_border);

/**
 * @brief Checks collision between object and wall
 * @param object Sprite pointer to the sprite object
 * @param new_x Top left corner x position that the object is trying to move into
 * @param new_y Top left corner y position that the object is trying to move into
 * @param info Array of bools that will be filled, if position is true then object hit corresponding wall
*/
bool detect_wall_collision(Sprite *object, int new_x, int new_y, bool info[]);

/**
 * @brief Checks if item collided with the middle of the field
 * @param object Sprite pointer to the sprite object
 * @param new_x Top left corner x position that the object is trying to move into
 * @param new_y Top left corner y position that the object is trying to move into
*/
bool detect_middle_field_collision(Sprite *object, int new_x, int new_y);

/**
 * @brief This function detect if the ball hit some of the beacons
 * @param object Sprite pointer to the sprite object that collided with the beacon
*/
bool detect_beacon_collision(Sprite* object, int new_x, int new_y);

/**
 * @brief Handles collision between puck and Ball
 * @param object Sprite pointer to the sprite object that collided with the ball
*/
void ball_collision(Sprite *object);

/**
 * @brief Makes object1 bounce off object2
 * @param object1 Sprite pointer to the object that will bounce
 * @param object2 Sprite pointer to the object that was hit
*/
void bounce_off(Sprite* object1, Sprite* object2);

/**
 * @brief This function dictates how an object that hit a wall should behave
 * @param object Sprite pointer to the sprite object that collided with the ball
 * @param info Bool array of size 4 that represents the walls
*/
void handle_play_area_collision(Sprite* object, bool info[]);

#endif

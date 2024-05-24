#include "physics.h"

void move(Sprite* object, int16_t x, int16_t y) {
    int new_x = object->x + x;
    int new_y = object->y - y;

    unsigned h_start = horizontal_start;
    unsigned h_end = horizontal_end;

    unsigned v_start = vertical_start;
    unsigned v_end = vertical_end;

    if (new_x + object->width > (int)h_end || new_x  < (int)h_start) return;
    if (new_y + object->height > (int)v_end || new_y < (int)v_start) return; 

    
    object->x = new_x;
    object->y = new_y;
}

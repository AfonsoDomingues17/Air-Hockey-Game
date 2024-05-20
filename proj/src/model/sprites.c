#include "sprites.h"

Sprite *create_sprite(xpm_map_t xpm, int x, int y) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    if (sp == NULL) return NULL;

    
    // read the sprite pixmap
    xpm_image_t img;
    sp->map = xpm_load(xpm, XPM_8_8_8_8, &img);
    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }
    
    sp->width = img.width; sp->height = img.height;
    sp->xspeed = 0; sp->yspeed = 0;
    sp->x = x; sp->y = y;
    sp->selected = false;
    sp->visibility = true;
    
    return sp;
}

void *delete_sprite(Sprite *sp) {
    if (sp == NULL) return NULL;
    if (sp->map) free(sp->map);
    free(sp);
    sp = NULL;
    return sp;
}

void move(Sprite* object, int16_t x, int16_t y) {
    int new_x = object->x + x;
    int new_y = object->y - y;

    unsigned h_start = 273;
    unsigned h_end = 879;

    unsigned v_start = 21;
    unsigned v_end = 843;

    if (new_x + object->width > (int)h_end || new_x  < (int)h_start) return;
    if (new_y + object->height > (int)v_end || new_y < (int)v_start) return; 

    object->x = new_x;
    object->y = new_y;
}

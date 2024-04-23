#include "sprites.h"
#include "../devices/graphics/graphics.h"

Sprite *create_sprite(xpm_map_t xpm) {
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    xpm_image_t img;
    if (sp == NULL) return NULL;
    // read the sprite pixmap
    sp->map = draw_xpm(xpm, XPM_INDEXED, &img);
    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }
    sp->width = img.width; sp->height = img.height;
    return sp;
}

void *delete_sprite(Sprite *sp) {
    if (sp == NULL) return;
    if (sp->map) free(sp->map);
    free(sp);
    sp = NULL;
}
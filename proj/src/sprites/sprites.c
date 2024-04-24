#include "sprites.h"
#include "../devices/graphics/graphics.h"
#include "../assets/xpm_background.h"

Sprite *background;

Sprite *create_sprite(xpm_map_t xpm) {
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    if (sp == NULL) return NULL;

    // read the sprite pixmap
    xpm_image_t img;
    // direct mode -> XPM_8_8_8_8
    sp->map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);
    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }
    sp->width = img.width; sp->height = img.height;
    return sp;
}


void *delete_sprite(Sprite *sp) {
    if (sp == NULL) return NULL;
    if (sp->map) free(sp->map);
    free(sp);
    sp = NULL;
    return sp;
}

void loader_sprite() {
    background = create_sprite((xpm_map_t) xpm_background);
}

void unloader_sprite() {
    delete_sprite(background);
}

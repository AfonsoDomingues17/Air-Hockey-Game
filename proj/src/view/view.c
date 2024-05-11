#include "view.h"

extern MainStateMachine mainState;

extern unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */
extern unsigned h_res;
extern unsigned v_res;
extern uint8_t *main_buffer; //main buffer 
extern uint8_t *secondary_buffer; //secondary buffer
extern unsigned int vram_size; 

xpm_image_t red_puck;
xpm_image_t blue_puck;
xpm_image_t game_background; /* Struct with background info like color */
xpm_image_t menu_background; /* Struct with background info like color */

void (draw_frame)() {
  switch(mainState) {
    case MAIN_MENU:
      vg_draw_background((xpm_map_t) menu, &menu_background);
      //vg_draw_sprite(mouse);
      if(!menu_button->selected) vg_draw_sprite(menu_button);
      else vg_draw_sprite(menu_2button);
      if(!menu_3button->selected) vg_draw_sprite(menu_3button);
      else vg_draw_sprite(menu_4button);
      if(!menu_5button->selected) vg_draw_sprite(menu_5button);
      else vg_draw_sprite(menu_6button);


      break;
    case GAME:
      vg_draw_background((xpm_map_t) xpm_background, &game_background);
      vg_draw_sprite(mouse);
      break;
    default:
      break;
  }
}

void(swap_buffers)(){
  memcpy(main_buffer,secondary_buffer,vram_size);
}



int (vg_draw_background)(xpm_map_t xpm, xpm_image_t *img) {
  if (img->bytes == NULL) {
    if (xpm_load(xpm, XPM_8_8_8_8, img) == NULL) return 1;
  }

  /* Draw Sprite */
  uint8_t *sprite = img->bytes;
  for (uint16_t line = 0; line < img->height; line++) {
    uint8_t *ptr = secondary_buffer;
    ptr += (line*h_res) * bytes_per_pixel;

    memcpy(ptr, sprite, img->width*bytes_per_pixel);
    ptr += img->width * bytes_per_pixel;
    sprite += img->width * bytes_per_pixel;
  }

  //int disk_width = 50, disk_height = 50;
  int redpuck_x = 100, redpuck_y = 100;
  int bluepuck_x = 100, bluepuck_y = 300;
  //int disk_x = (h_res - disk_width) / 2;
  //int disk_y = (v_res - disk_height) / 2;

  draw_puck(redpuck_x, redpuck_y, 1, red_puck);
  draw_puck(bluepuck_x, bluepuck_y, 2, blue_puck);

  return 0;
}

int (draw_puck)(int x, int y, int player, xpm_image_t *puck) {

     if (puck->bytes == NULL) return 1;
  
    uint8_t *sprite = puck->bytes;
    for (uint16_t line = 0; line < puck->height; line++) {
        uint8_t *ptr = secondary_buffer;
        ptr += ((line + y) * h_res + x) * bytes_per_pixel;

        memcpy(ptr, sprite, puck->width * bytes_per_pixel);
        ptr += puck->width * bytes_per_pixel;
        sprite += puck->width * bytes_per_pixel;
    }

    return 0;
}

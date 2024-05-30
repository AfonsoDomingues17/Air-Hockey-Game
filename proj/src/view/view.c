#include "view.h"

extern unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */
extern unsigned h_res;
extern unsigned v_res;
extern uint8_t *main_buffer; //main buffer 
extern uint8_t *secondary_buffer; //secondary buffer
extern unsigned int vram_size; 

extern MainStateMachine mainState;

xpm_image_t game_background; /* Struct with background info like color */
xpm_image_t menu_background; /* Struct with background info like color */

void (draw_frame)() {
  switch(mainState) {
    case MAIN_MENU:
      vg_draw_background((xpm_map_t) menu, &menu_background);
      if(!start_button_unselected->selected) vg_draw_sprite(start_button_unselected);
      else vg_draw_sprite(start_button_selected);
      if(!leaderboard_button_unselected->selected) vg_draw_sprite(leaderboard_button_unselected);
      else vg_draw_sprite(leaderboard_button_selected);
      if(!exit_button_unselected->selected) vg_draw_sprite(exit_button_unselected);
      else vg_draw_sprite(exit_button_selected);
      break;
    case GAME:
      vg_draw_background((xpm_map_t) xpm_background, &game_background);
      vg_draw_sprite(redpuck);
      vg_draw_sprite(bluepuck);
      vg_draw_sprite(mouse);
      vg_draw_sprite(Ball);
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

  return 0;
}

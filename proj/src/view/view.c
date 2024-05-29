#include "view.h"

extern unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */
extern unsigned h_res;
extern unsigned v_res;
extern uint8_t *main_buffer; //main buffer 
extern uint8_t *secondary_buffer; //secondary buffer
extern unsigned int vram_size; 

extern MainStateMachine mainState;

xpm_image_t game_background; /* Struct with background info like color */
xpm_image_t menu_background; /* Struct with menu info like color */
xpm_image_t youwon_popOut; /* Struct with youWON info like color */
xpm_image_t youlost_popOut; /* Struct with youLOST info like color */
extern unsigned int time_remaining;

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
      vg_draw_sprite(time_sep);
      vg_draw_time(time_remaining, 10, 10);
      break;
    case WIN:
      vg_drawn_popOut((xpm_map_t) youwon, &youwon_popOut);
    case LOST:
      vg_drawn_popOut((xpm_map_t) youlost, &youlost_popOut);
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

int (vg_drawn_popOut)(xpm_map_t xpm, xpm_image_t *img) {
  if (img->bytes == NULL) {
    if (xpm_load(xpm, XPM_8_8_8_8, img) == NULL) return 1;
  }

  /* Draw Sprite */
  uint8_t *sprite = img->bytes;
  for (uint16_t line = (get_v_res()/2 - img->height/2); line < img->height + (get_v_res()/2 - img->height/2); line++) {
    uint8_t *ptr = secondary_buffer + (get_h_res()/2 - img->width/2)*bytes_per_pixel;
    ptr += (line*h_res) * bytes_per_pixel;

    memcpy(ptr, sprite, img->width*bytes_per_pixel);
    ptr += img->width * bytes_per_pixel;
    sprite += img->width * bytes_per_pixel;
  }

  return 0;
}

void (vg_draw_time)(unsigned int time, int x, int y) {
  unsigned int minutes = time / 60;
  unsigned int seconds = time % 60;

  numbers[minutes / 10]->x = x;
  numbers[minutes / 10]->y = y;
  vg_draw_sprite(numbers[minutes / 10]);

  numbers[minutes % 10]->x = x + 50;
  numbers[minutes % 10]->y = y;
  vg_draw_sprite(numbers[minutes % 10]);

  numbers[seconds / 10]->x = x + 3 * 50;
  numbers[seconds / 10]->y = y;
  vg_draw_sprite(numbers[seconds / 10]);
  
  numbers[seconds % 10]->x = x + 4 * 50;
  numbers[seconds % 10]->y = y;
  vg_draw_sprite(numbers[seconds % 10]);
} 

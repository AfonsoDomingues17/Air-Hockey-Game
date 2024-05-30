#include "view.h"

extern unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */
extern unsigned h_res;
extern unsigned v_res;
extern uint8_t *main_buffer; //main buffer 
extern uint8_t *secondary_buffer; //secondary buffer
extern unsigned int vram_size; 
extern day_time day_time_info;

extern MainStateMachine mainState;

xpm_image_t game_background; /* Struct with background info like color */
xpm_image_t menu_background; /* Struct with menu info like color */
xpm_image_t youwon_popOut; /* Struct with youWON info like color */
xpm_image_t youlost_popOut; /* Struct with youLOST info like color */
xpm_image_t tie_popOut; /* Struct with tie info like color */
extern unsigned int time_remaining;
extern int player_1;
extern int player_2;

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
      vg_draw_rtc(&day_time_info, 10, 10);
      vg_draw_sprite(two_points);
      vg_draw_sprite(two_points2);
      vg_draw_sprite(day_sep);
      vg_draw_sprite(day_sep2);
      break;
    case GAME:
      vg_draw_background((xpm_map_t) xpm_background, &game_background);
      vg_draw_sprite(redpuck);
      vg_draw_sprite(bluepuck);
      vg_draw_sprite(mouse);
      vg_draw_sprite(Ball);
      vg_draw_sprite(time_sep);
      vg_draw_sprite(time_title);
      vg_draw_sprite(points_title);
      vg_draw_sprite(points_sep);
      vg_draw_time(time_remaining, 35, 420);
      vg_draw_points(player_1, player_2, 960, 425);
      break;
    case WIN:
      vg_drawn_popOut((xpm_map_t) youwon, &youwon_popOut);
      if(!leave_button_unselected->selected) vg_draw_sprite(leave_button_unselected);
      else vg_draw_sprite(leave_button_selected);
      if(!play_again_button_unselected->selected) vg_draw_sprite(play_again_button_unselected);
      else vg_draw_sprite(play_again_button_selected);
      break;
    case LOST:
      vg_drawn_popOut((xpm_map_t) youlost, &youlost_popOut);
      if(!leave_button_unselected->selected) vg_draw_sprite(leave_button_unselected);
      else vg_draw_sprite(leave_button_selected);
      if(!play_again_button_unselected->selected) vg_draw_sprite(play_again_button_unselected);
      else vg_draw_sprite(play_again_button_selected);
      break;
    case TIE:
      vg_drawn_popOut((xpm_map_t) tie, &tie_popOut);
      if(!leave_button_unselected->selected) vg_draw_sprite(leave_button_unselected);
      else vg_draw_sprite(leave_button_selected);
      if(!play_again_button_unselected->selected) vg_draw_sprite(play_again_button_unselected);
      else vg_draw_sprite(play_again_button_selected);
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

  numbers[minutes % 10]->x = x + 40;
  numbers[minutes % 10]->y = y;
  vg_draw_sprite(numbers[minutes % 10]);

  numbers[seconds / 10]->x = x + 3 * 40 - 15;
  numbers[seconds / 10]->y = y;
  vg_draw_sprite(numbers[seconds / 10]);
  
  numbers[seconds % 10]->x = x + 4 * 40 - 15;
  numbers[seconds % 10]->y = y;
  vg_draw_sprite(numbers[seconds % 10]);
} 

void (vg_draw_points)(int player_1, int player_2, int x, int y) {
  numbers_red[player_1]->x = x;
  numbers_red[player_1]->y = y;
  vg_draw_sprite(numbers_red[player_1]);
  numbers_blue[player_2]->x = x + 105;
  numbers_blue[player_2]->y = y;
  vg_draw_sprite(numbers_blue[player_2]);
}

void (vg_draw_rtc)(day_time *time, int x, int y) {
  // hours
  numbers[time->h / 10]->x = x;
  numbers[time->h / 10]->y = y;
  vg_draw_sprite(numbers[time->h / 10]);
  numbers[time->h % 10]->x = x + 35;
  numbers[time->h % 10]->y = y;
  vg_draw_sprite(numbers[time->h % 10]);

  // minutes
  numbers[time->m / 10]->x = x + 35 * 3;
  numbers[time->m / 10]->y = y;
  vg_draw_sprite(numbers[time->m / 10]);
  numbers[time->m % 10]->x = x + 35 * 4;
  numbers[time->m % 10]->y = y;
  vg_draw_sprite(numbers[time->m % 10]);

  // seconds
  numbers[time->s / 10]->x = x + 35 * 6;
  numbers[time->s / 10]->y = y;
  vg_draw_sprite(numbers[time->s / 10]);
  numbers[time->s % 10]->x = x + 35 * 7;
  numbers[time->s % 10]->y = y;
  vg_draw_sprite(numbers[time->s % 10]);

  // day
  numbers[time->dd / 10]->x = x + 35 * 10;
  numbers[time->dd / 10]->y = y;
  vg_draw_sprite(numbers[time->dd / 10]);
  numbers[time->dd % 10]->x = x + 35 * 11;
  numbers[time->dd % 10]->y = y;
  vg_draw_sprite(numbers[time->dd % 10]);

  // month
  numbers[time->mm / 10]->x = x + 5 + 35 * 13;
  numbers[time->mm / 10]->y = y;
  vg_draw_sprite(numbers[time->mm / 10]);
  numbers[time->mm % 10]->x = x + 5 +  35 * 14;
  numbers[time->mm % 10]->y = y;
  vg_draw_sprite(numbers[time->mm % 10]);

  // year
  numbers[2]->x = x + 5 + 35 * 16;
  numbers[2]->y = y;
  vg_draw_sprite(numbers[2]);
  numbers[0]->x = x + 5 + 35 * 17;
  numbers[0]->y = y;
  vg_draw_sprite(numbers[0]);
  numbers[time->yyyy / 10]->x = x + 5 + 35 * 18;
  numbers[time->yyyy / 10]->y = y;
  vg_draw_sprite(numbers[time->yyyy / 10]);
  numbers[time->yyyy % 10]->x = x + 5 + 35 * 19;
  numbers[time->yyyy % 10]->y = y;
  vg_draw_sprite(numbers[time->yyyy % 10]);

} 

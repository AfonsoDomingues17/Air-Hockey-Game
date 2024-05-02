#include "view.h"

extern MainStateMachine mainState;

void (draw_frame)() {
  switch(mainState) {
    case RUNNING:
      vg_draw_background((xpm_map_t) xpm_background);
      vg_draw_sprite(mouse);
      break;
    default:
      break;
  }
}

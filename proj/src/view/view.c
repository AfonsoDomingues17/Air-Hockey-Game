#include "view.h"

extern MainStateMachine mainState;

void (draw_frame)() {
  switch(mainState) {
    case RUNNING:
      vg_draw_xpm((xpm_map_t) xpm_background, 0, 0);
      break;
    default:
      break;
  }
}

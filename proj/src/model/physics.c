#include "physics.h"

void move(Sprite* object, int16_t x, int16_t y, unsigned layer) {
    int new_x = object->x + x;
    int new_y = object->y - y;

    unsigned h_start = horizontal_start;
    unsigned h_end = horizontal_end;

    unsigned v_start = vertical_start;
    unsigned v_end = vertical_end;

    if (new_x + object->width > (int)h_end || new_x  < (int)h_start) return;
    if (new_y + object->height > (int)v_end || new_y < (int)v_start) return; 

    if (detect_collision_in_layer(object, new_x, new_y, layer)) {
      return;
    } else {
      object->x = new_x;
      object->y = new_y;
    }
}

bool detect_collision_in_layer(Sprite* object, int new_x, int new_y, unsigned layer) {
  switch (layer)
  {
  case 1:
    if (detec_collision(object, new_x, new_y, redpuck)) return true;
    if (detec_collision(object, new_x, new_y, bluepuck)) return true;
    if (detec_collision(object, new_x, new_y, Ball)) {
      return true;
    }
  default:
    return false;;
  }
}

bool detec_collision(Sprite* object1, int new_x, int new_y, Sprite* object2) {
  if (object1 == object2 || object1 == NULL || object2 == NULL) return false;

  int left_border = max(new_x, object2->x);
  int right_border = min(new_x+object1->width, object2->x+object2->width);
  int top_border = max(new_y, object2->y);
  int bottom_border = min(new_y+object1->height, object2->y+object2->height);

  return ((right_border > left_border) && (top_border < bottom_border));
}

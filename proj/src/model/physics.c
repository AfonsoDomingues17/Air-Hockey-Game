#include "physics.h"

extern unsigned bytes_per_pixel;

void move(Sprite* object, int16_t x, int16_t y, unsigned layer) {
    int new_x = object->x + x;
    int new_y = object->y - y;

    if (detect_collision_in_layer(object, new_x, new_y, layer)) {
      return;
    } else {
      object->x = new_x;
      object->y = new_y;
      object->xspeed = x;
      object->yspeed = y;
    }
}

bool detect_collision_in_layer(Sprite* object, int new_x, int new_y, unsigned layer) {
  switch (layer)
  {
  case 0: // Collision Layer for Mouse
    if (detect_wall_collision(object, new_x, new_y)) return true;
    break;
  case 1: // Collision Layer for Ball object
    if (detect_wall_collision(object, new_x, new_y)) {
      return true;
    }
    if (detect_collision(object, new_x, new_y, redpuck)) return true;
    if (detect_collision(object, new_x, new_y, bluepuck)) return true;
    break;
  case 2: // Collision Layer for Pucks
    if (detect_wall_collision(object, new_x, new_y)) return true;
    if (detect_middle_field_collision(object, new_x, new_y)) return true;
    if (detect_collision(object, new_x, new_y, Ball)) {
      ball_collision(object);
      return true;
    }
    break;
  default:
    return false;
  }
  return false;
}

bool detect_collision(Sprite* object1, int new_x, int new_y, Sprite* object2) {
  if (object1 == object2 || object1 == NULL || object2 == NULL) return false;

  int left_border = max(new_x, object2->x);
  int right_border = min(new_x+object1->width, object2->x+object2->width);
  int top_border = max(new_y, object2->y);
  int bottom_border = min(new_y+object1->height, object2->y+object2->height);

  if ((right_border >= left_border) && (top_border <= bottom_border)) {
    // Sprite Squares have overlapped move on to pixel detection
    return pixel_detection(object1, new_x, new_y, object2, left_border, right_border, top_border, bottom_border);
  }

  return false;
}

bool pixel_detection(Sprite* object1, int new_x, int new_y, Sprite* object2, int left_border, int right_border, int top_border, int bottom_border) {
  // Loop over every pixel inside the region
  for (int x = left_border; x < right_border; x++) {
    for (int y = top_border; y < bottom_border; y++) {
      // Get color of pixel in object1
      int pixel1_y = y - new_y;
      int pixel1_x = x - new_x;
      int position1 = ((object1->width*pixel1_y) + pixel1_x) * bytes_per_pixel;
      uint32_t color1 = 0;
      uint8_t* colors1 = object1->map;
      colors1 += position1;
      memcpy(&color1, colors1, bytes_per_pixel);
      // Get color of pixel in object2
      int pixel2_y = y - object2->y;
      int pixel2_x = x - object2->x;
      int position2 = ((object2->width*pixel2_y) + pixel2_x) * bytes_per_pixel;
      uint32_t color2 = 0;
      uint8_t* colors2 = object2->map;
      colors2 += position2;
      memcpy(&color2, colors2, bytes_per_pixel);
      // Check if there is overlap
      if (color1 != TRANSPARENT && color2 != TRANSPARENT) {
        return true;
      }
    }
  }

  return false;
}

bool detect_wall_collision(Sprite *object, int new_x, int new_y) {
  if (new_x + object->width > horizontal_end || new_x  < horizontal_start) return true;
  if (new_y + object->height > vertical_end || new_y < vertical_start) return true;
  return false; 
}

bool detect_middle_field_collision(Sprite *object, int new_x, int new_y) {
  if (object->y < middle_field) { // Above middle field
    if (new_y < object->y) return false; // If moving up then there is no collision
    return (new_y + object->height > middle_field); // If moving down check treshold
  }
  // Bellow middle Field
  if (new_y > object->y) return false; // If moving down then there is no collision
  return (new_y < middle_field); // If moving up check treshold
}

void ball_collision(Sprite *object) {
  Ball->xspeed = object->xspeed;
  Ball->yspeed = object->yspeed;
}

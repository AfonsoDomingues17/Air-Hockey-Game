#include <lcom/lcf.h>

#include "graphics.h"

int (graphics_set_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.ax = VBE_CALL | SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
  r.bx = SET_LINEAR_BIT | mode; // set bit 14: linear framebuffer 
  r.intno = VBE_INT;
  if( sys_int86(&r) ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;
}

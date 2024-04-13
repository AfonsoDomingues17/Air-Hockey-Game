#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "i9000.h"

int set_VBE_mode(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.intno = 0x10;
  r.ax = 0x4F02;
  r.bx = BIT(14) | mode;

  if( sys_int86(&r) != OK ) {
  printf("set_vbe_mode: sys_int86() failed \n");
  return 1;
  }
  
  return 0;
}

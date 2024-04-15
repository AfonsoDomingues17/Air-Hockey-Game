#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "i9000.h"

int set_VBE_mode(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.intno = 0x10;
  r.ah = 0x4F;
  r.al = 0x02;
  r.bx = BIT(14) | mode;

  if( sys_int86(&r) != OK ) {
  printf("set_vbe_mode: sys_int86() failed \n");
  return 1;
  }
  
  return 0;
}

static uint16_t hres;
static uint16_t vres;
static void *video_mem;
static int bytesPerPixel;

void *(vg_init)(uint16_t mode) {
  vbe_mode_info_t vmi_p;

  vbe_get_mode_info(mode, &vmi_p);

  bytesPerPixel = (vmi_p.BitsPerPixel + 7) / 8;
  hres = vmi_p.XResolution;
  vres = vmi_p.YResolution;
  unsigned int vram_size = bytesPerPixel * vres * hres;
  unsigned int vram_base = vmi_p.PhysBasePtr;
  int r;

  struct minix_mem_range mr;

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) return NULL;

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED) return NULL;

  if (set_VBE_mode(mode)) return NULL;

  return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x + len > hres) len = hres - x;

  uint8_t* p = video_mem;
  //printf("Ptr: %x, Color: %x\n", p, color);
  //printf("BytesPerPixel %d\n", bytesPerPixel);
  p += (y*hres + x)*bytesPerPixel;
  //printf("Ptr: %x, Color: %x\n", p, color);

  for (int k = 0 ; k < len ; k++) {
    memcpy(p, &color, bytesPerPixel);
    p += bytesPerPixel;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (y + height > vres) height = vres - y;
  
  for (int k = 0; k < height ; k++) {
    vg_draw_hline(x, y+k, width, color);
  }
  return 0;
}

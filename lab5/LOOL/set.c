#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

//set vbe
int set_vbe(uint16_t mode) {
  reg86_t t;
  memset(&t, 0, sizeof(t));

  t.intno = 0x10;
  t.ax =  0x4F02;
  t.bx =  1<<14 | mode;

  if (sys_int86(&t) != OK) return 1;
  return 0;
}

//map
void *map(uint16_t mode) {
  lpv_mode_info_t info;

  lpv_get_mode_info(mode, &info);

  int r;
  struct minix_mem_range mr; 
  unsigned int vram_base = info.physical; 
  unsigned int bytes = (info.bit + 7) / 8; 
  unsigned int vram_size = bytes * xres * yres;

  void *video_mem; 
  
  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
  panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
  panic("couldn’t map video memory");

}

//Normalizar cor
redmask = (1 << vmi.redsize) - 1;
red = (first & (redmask << 18)) >> 18
color = red << redposition | green << greenposition | blue << blueposition;


bluemask = (1 << vmi.bluemask) - 1;
greenmask = (1 << vmi.greenmask) - 1;
if (memory_model == 0x04) {
        color = (first + (row * no_rectangles + col) * step) % (1 << BitsPerPixel);
      }
else {
  unsigned char red = (first & (redmask << redposition)) >> redposition;
  red = (red + col * step) % (1 << vmi_p.RedMaskSize);

  unsigned char green = (first & (greenmask << greenposition)) >> greenposition;
  green = (green + row * step) % (1 << vmi_p.GreenMaskSize);

  unsigned char blue = (first & (bluemask << blueposition)) >> blueposition;
  blue = (blue + (col + row) * step) % (1 << vmi_p.BlueMaskSize);

  color = (red << redposition) | (green << greenposition) | (blue << blueposition);
}

//exit
int vg_exit() {
reg86_t reg86;
memset(&reg86, 0, sizeof(reg86));

reg86.intno = 0x10;
reg86.ah = 0x00;
reg86.al = 0x03;
if( sys_int86(&reg86) != OK ) {
  printf("vg_exit(): sys_int86() failed \n");
  return 1;
}
return 0;
}

//desenhar linha horizontal ou vertical

enum lpv_dir_t {
lpv_hor, // horizontal line
lpv_vert // vertical line
};

int draw_line(int x, int y, int len, uint32_t color, enum lpv_dir_t) {
  if (lpv_dir_t == lpv_hor) {
    if (x + len > xres) len = xres - x;

    p = video_map_phys(fisical adress, len);

    p += (y*hres + x) * bytes;

    for (int k = 0; k < len; k++) {
      memcpy(p, &color, bytes);
      p += bytes;
    }
  }
  else {
    if (y + len > yres) len = yres - y;

    p = video_map_phys(fisical adress, len);
  }
}

int draw_line(int x, int y, int len, uint32_t color) {
  if (mode == 1) {

  }

  uint8_t* p = asdasda();

  p += (y*hres + x) * bytes;

  for (int k = 0; k< len; k++) {
    memcpy(p, &color, bytes);
    p += bytes;
  }
}
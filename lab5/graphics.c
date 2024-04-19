#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "i9000.h"
#include "keyboard.h"
#include "i8254_i8042.h"

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
static int BitsPerPixel;

static unsigned char memory_model;

static unsigned char redposition;
static unsigned char blueposition;
static unsigned char greenposition;

static unsigned char redmask;
static unsigned char bluemask;
static unsigned char greenmask;


void *(vg_init)(uint16_t mode) {
  vbe_mode_info_t vmi_p;

  vbe_get_mode_info(mode, &vmi_p);

  memory_model = vmi_p.MemoryModel;
  BitsPerPixel = vmi_p.BitsPerPixel;
  redposition = vmi_p.RedFieldPosition;
  blueposition = vmi_p.BlueFieldPosition;
  greenposition = vmi_p.GreenFieldPosition;

  redmask = vmi_p.RedMaskSize;
  bluemask = vmi_p.BlueMaskSize;
  greenmask = vmi_p.GreenMaskSize;

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

uint8_t extern scancode;
bool extern failed;
int extern count_timer;

int wait_for_esc() {
  int re, ipc_status;
  message msg;
  uint8_t bit_no;
  uint8_t size;

  if (keyboard_subscribe(&bit_no)) return 1;
  uint8_t byte[2];
  bool is_2byte = false;
  
  while (scancode != ESC) {
    if ((re = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
      printf("driver_receive failed with: %d", re);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) 
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & bit_no) { 
          kbc_ih();
          if (failed) continue;

          if (scancode == IS_2BYTECODE) {
            byte[0] = scancode;
            is_2byte = true;
            break;
          }

          if (is_2byte) {
            size = 2;
            is_2byte = false;
            byte[1] = scancode;
          }
          else {
            byte[0] = scancode;
            size = 1;
          }
        }
        break;
      
      default:
        break;
      }
    }
  }
  if (keyboard_unsubscribe()) return 1;
  return 0;
}

int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  int width = hres/no_rectangles; 
  int height = vres/no_rectangles;
  uint32_t color;
  for (int y = 0; y < no_rectangles*height; y += height) {
    for (int x = 0; x < no_rectangles*width; x += width) {
      int row = y / height;
      int col = x / width;
      if (memory_model == 0x04) {
        color = (first + (row * no_rectangles + col) * step) % (1 << BitsPerPixel);
      }
      else {
        unsigned char red = (first & redmask) >> redposition;
        unsigned char green = (first & greenmask) >> greenposition; 
        unsigned char blue = (first & bluemask) >> blueposition; 
        color = red | green | blue;
      }
      if (vg_draw_rectangle(x, y, width, height, color)) return 1;
    }
  }
  return 0;
}

int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint8_t* map = xpm_load(xpm, XPM_INDEXED, &img);
  if (map == NULL) return 1;

  for (int k = 0; k < img.height; k++) {
      uint8_t * ptr = video_mem;
      ptr += ((y+k)*hres) + x;

      memcpy(ptr, map, img.width);
      ptr += img.width;
      map += img.width;
  }
  return 0;
}

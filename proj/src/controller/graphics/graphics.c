#include <lcom/lcf.h>

#include "graphics.h"



uint8_t *main_buffer; //main buffer 
uint8_t *secondary_buffer; //secondary buffer
unsigned int vram_size;

unsigned h_res; /* Horizontal resolution in pixels */
static unsigned v_res; /* Vertical resolution in pixels */
unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */
static unsigned memory_model; /* Memory Model (Direct or Indexed) */

static unsigned red_mask_size; /* Size of direct color red mask in bits */
static unsigned red_field_position;	/* Bit position of lsb of red mask */
static unsigned green_mask_size; /* Size of direct color green mask in bits */
static unsigned green_field_position;	/* Bit position of lsb of green mask */
static unsigned blue_mask_size; /* Size of direct color blue mask in bits */
static unsigned blue_field_position;	/* Bit position of lsb of blue mask */

static unsigned int vram_base; /* VRAM’s physical addresss */



int (graphics_set_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.ax = VBE_CALL | SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
  r.bx = (SET_LINEAR_BIT | mode); // set bit 14: linear framebuffer 
  r.intno = VBE_INT;
  if( sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;
}

int (map_phys_mem)(uint16_t mode) {
  int r;
  struct minix_mem_range mr; /* physical memory range */
  
  vram_size = h_res * v_res * bytes_per_pixel; /* VRAM’s size, but you can use the frame-buffer size, instead */
  
  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  
  /* Map memory */
  main_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(main_buffer == MAP_FAILED)
    panic("couldn't map video memory");

  secondary_buffer = (uint8_t *) malloc(vram_size);
  return 0;
}

void* (vg_init)(uint16_t mode) {
  /* Initialize static global variables */
  vbe_mode_info_t vmi_p;

  if (vbe_get_mode_info(mode, &vmi_p)) return NULL;
  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bytes_per_pixel = (vmi_p.BitsPerPixel+7)/8;
  memory_model = vmi_p.MemoryModel;

  red_mask_size = vmi_p.RedMaskSize; 
  red_field_position = vmi_p.RedFieldPosition;
  green_mask_size = vmi_p.GreenMaskSize;
  green_field_position = vmi_p.GreenFieldPosition;
  blue_mask_size = vmi_p.BlueMaskSize;
  blue_field_position = vmi_p.BlueFieldPosition;

  vram_base = vmi_p.PhysBasePtr;

  /* Map Physical Memory */
  if (map_phys_mem(mode)) return NULL;

  /* Set VG Mode */
  if (graphics_set_mode(mode)) return NULL;

  return main_buffer;
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  uint8_t *ptr = secondary_buffer;
  ptr += (y*h_res + x)*bytes_per_pixel;
  if (x + len > h_res) len = h_res-x;
  for (int i = 0; i < len; i++, ptr+=bytes_per_pixel) {
    memcpy(ptr, &color, bytes_per_pixel);
  }

  return 0;
}

uint32_t (create_mask)(unsigned int size, unsigned int lsb) {
  uint32_t mask = (1 << size) - 1;
  mask <<= lsb;
  return mask;
}

uint32_t (get_color_component)(uint32_t color, uint32_t mask_size, uint32_t lsb) {
  return (color & create_mask(mask_size, lsb)) >> lsb;
}

int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* Convert XPM to PixMap */
  xpm_image_t img;
  uint8_t* sprite = xpm_load(xpm, XPM_8_8_8_8, &img);
  if (sprite == NULL) return 1;

  /* Draw Sprite */
  for (uint16_t line = 0; line < img.height; line++) {
    uint8_t *ptr = secondary_buffer;
    ptr += ((y+line)*h_res + x) * bytes_per_pixel;

    memcpy(ptr, sprite, img.width*bytes_per_pixel);
    ptr += img.width * bytes_per_pixel;
    sprite += img.width * bytes_per_pixel;
  }

  return 0;
}


int (vg_draw_sprite)(Sprite *sprite) {
  if (sprite == NULL || sprite->map == NULL) return 1;
  if(sprite->visibility){
  /* Draw Sprite */
  uint8_t* colors = sprite->map;
  for (uint16_t line = 0; line < sprite->height; line++) {
    uint8_t *ptr = secondary_buffer;
    ptr += ((sprite->y+line)*h_res + sprite->x) * bytes_per_pixel;

    for (int i = 0; i < sprite->width; i++, ptr+=bytes_per_pixel, colors+=bytes_per_pixel) {
      // Check if color is TRANSPARENT
      uint32_t color = 0;
      memcpy(&color, colors, bytes_per_pixel);
      if (color == TRANSPARENT) continue;
      // Paint pixel
      memcpy(ptr, colors, bytes_per_pixel);
    }
  }
  }

  return 0;
}

unsigned (get_h_res)() {
  return h_res;
}

unsigned (get_v_res)() {
  return v_res;
}

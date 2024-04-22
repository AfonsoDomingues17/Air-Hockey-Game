#include <lcom/lcf.h>

#include "graphics.h"

static void *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned memory_model; /* Memory Model (Direct or Indexed) */

static unsigned red_mask_size; /* size of direct color red mask in bits */
static unsigned red_field_position;	/* bit position of lsb of red mask */
static unsigned green_mask_size; /* size of direct color green mask in bits */
static unsigned green_field_position;	/* bit position of lsb of green mask */
static unsigned blue_mask_size; /* size of direct color blue mask in bits */
static unsigned blue_field_position;	/* bit position of lsb of blue mask */

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
  
  unsigned int vram_size = h_res * v_res * (bits_per_pixel+7)/8; /* VRAM’s size, but you can use the frame-buffer size, instead */
  
  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
    panic("couldn’t map video memory");
  
  return 0;
}

void* (vg_init)(uint16_t mode) {
  /* Initialize static global variables */
  vbe_mode_info_t vmi_p;

  if (vbe_get_mode_info(mode, &vmi_p)) return NULL;
  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
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

  return video_mem;
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  unsigned int bytes_per_pixel = (bits_per_pixel+7)/8;
  uint8_t *ptr = video_mem;
  ptr += (y*h_res + x)*bytes_per_pixel;
  if (x + len > h_res) len = h_res-x;
  for (int i = 0; i < len; i++, ptr+=bytes_per_pixel) {
    memcpy(ptr, &color, bytes_per_pixel);
  }

  return 0;
}

int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (x > h_res || y > v_res) return 1;
  
  if (y + height > v_res) height = v_res-y;
  for (uint16_t line = 0; line < height; line++) {
    if (vg_draw_hline(x, y+line, width, color)) return 1;
  }
  
  return 0;
}	

uint32_t create_mask(unsigned int size, unsigned int lsb) {
  uint32_t mask = (1 << size) - 1;
  mask <<= lsb;
  return mask;
}

uint32_t (get_color_component)(uint32_t color, uint32_t mask_size, uint32_t lsb) {
  return (color & create_mask(mask_size, lsb)) >> lsb;
}

int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint16_t width = h_res/no_rectangles, height = v_res/no_rectangles;

  for (uint16_t y = 0; y < no_rectangles*height; y += height) {
    for (uint16_t x = 0; x < no_rectangles*width; x += width) {
      uint32_t color, col = (x/width), row = (y/height);
      if (memory_model != INDEXED_MODEL) {
        uint32_t red, green, blue, first_red, first_green, first_blue;
        first_red = get_color_component(first, red_mask_size, red_field_position);
        first_green = get_color_component(first, green_mask_size, green_field_position);
        first_blue = get_color_component(first, blue_mask_size, blue_field_position);

        red = (first_red + col * step) % (1 << red_mask_size);
	      green = (first_green + row * step) % (1 << green_mask_size);
	      blue = (first_blue + (col + row) * step) % (1 << blue_mask_size);

        color = (red << red_field_position) | (green << green_field_position) | (blue << blue_field_position);
      }
      else color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);

      if (vg_draw_rectangle(x, y, width, height, color)) return 1;
    }
  }

  return 0;
}

int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* Convert XPM to PixMap */
  xpm_image_t img;
  uint8_t* sprite = xpm_load(xpm, XPM_INDEXED, &img);
  if (sprite == NULL) return 1;

  /* Draw Sprite */
  for (uint16_t line = 0; line < img.height; line++) {
    uint8_t *ptr = video_mem;
    ptr += ((y+line)*h_res + x);

    memcpy(ptr, sprite, img.width);
    ptr += img.width;
    sprite += img.width;
  }

  return 0;
}

int (vg_erase_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* Convert XPM to PixMap */
  xpm_image_t img;
  uint8_t* sprite = xpm_load(xpm, XPM_INDEXED, &img);
  if (sprite == NULL) return 1;
  
  /* Reset used pixels */
  for (uint16_t line = 0; line < img.height; line++) {
    uint8_t *ptr = video_mem;
    ptr += ((y+line)*h_res + x);

    uint8_t color_black = 0x00;
    for (int col = 0; col < img.width; col++, ptr++) {
      memcpy(ptr, &color_black, 1);
    }
  }

  return 0;
}

int (vg_redraw_xpm)(xpm_map_t xpm, uint16_t xo, uint16_t yo, uint16_t xi, uint16_t yi) {
  /* Reset used pixels */
  if (vg_erase_xpm(xpm, xo, yo)) return 1;

  /* Draw new Sprite */
  if (vg_draw_xpm(xpm, xi, yi)) return 1;

  return 0;
}

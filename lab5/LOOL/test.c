#include <lcom/lcf.h>
#include <stdint.h>

int set_vbe(uint16_t mode) {
  reg86_t r;

  r.intno = ###;
  r.ax = ####;
  r.bx = ### | mode;

  if (sysint86_t(&r) != OK) return 1;

  return 0;
}

void *init (uint16_t mode) {


int r;
struct minix_mem_range mr; /* physical memory range */
unsigned int vram_base; /* VRAM’s physical addresss */
unsigned int vram_size; /* VRAM’s size, but you can use*/



lpv_get_mode_info(mode, &lpv_mode_info_t);

unsigned int  bytes = (lpv_mode_info_t.bit + 7) / 8
vram_size = hres * vres * bytes;

vream_base = phisical adress;

void *video_mem; /* frame-buffer VM address */
/* Allow memory mapping */
mr.mr_base = (phys_bytes) vram_base;
mr.mr_limit = mr.mr_base + vram_size;
if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
panic("sys_privctl (ADD_MEM) failed: %d\n", r);
/* Map memory */

uint8_t *p = video_map_phys(mr.mr_base, vram_size);
video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
if(video_mem == MAP_FAILED)
panic("couldn’t map video memory");


}
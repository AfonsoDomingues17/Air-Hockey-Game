#include <lcom/lcf.h>

#include <stdint.h>

#ifdef LAB3
int counter = 0;
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  if(lsb == NULL) return 1;

  *lsb = (uint8_t)(val);
  
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  if(msb == NULL) return 1;

  *msb = (uint8_t)(val >> 8);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  if(value == NULL) return 1;
  uint32_t temp;
  if(sys_inb(port,&temp) != 0) return 1;
  *value = (uint8_t)(temp & 0xFF);
  return 0;
}

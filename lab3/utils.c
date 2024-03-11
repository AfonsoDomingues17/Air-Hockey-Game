#include <lcom/lcf.h>

#include <stdint.h>

uint32_t sys_inb_cnt = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  // Checking argument vadility
  if (lsb == NULL) return 1;
  // Getting LSB
  *lsb = val & 0xFF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  // Checking argument vadility
  if (msb == NULL) return 1;
  // Getting LSB
  *msb = (val & 0xFF00) >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1; // Checking arguments
  uint32_t byte;
  // Getting 4 byte info
  if (sys_inb(port, &byte)) {
    printf("%s returned an error!\n", __func__);
    return 1;
  }
  // Retriving relevant byte
  *value = byte & 0xFF;
  // Increase Count
  #ifdef LAB3
  sys_inb_cnt++;
  #endif
  return 0;
}

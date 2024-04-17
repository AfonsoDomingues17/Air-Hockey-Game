#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; // apontador tem de ser válido
  uint16_t mask = 0x00FF; // 8 bits menos significativos
  *lsb = mask & val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1; // apontador tem de ser válido
  uint16_t mask = 0xFF00; // 8 bits mais significativos
  *msb = (mask & val) >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) { // transforma 32 bit em 8 bit
  if (value == NULL) return 1; // apontador tem de ser válido
  uint32_t aux; 
  int ret = sys_inb(port, &aux); // aux = valor lido do port
  *value = 0xFF & aux; // atualizar value com os primeiros 8 bits do ret
  return ret;
}

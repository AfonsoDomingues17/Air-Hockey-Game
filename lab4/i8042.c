#include "i8042.h"

int (kbc_write_cmd)(uint8_t cmd) {
  uint8_t stat;
  for (int i = 0; i < KBC_RETRY_CNT; i++) {
    if (util_sys_inb(KBC_ST_REG, &stat)) return 1;
    /* loop while 8042 input buffer is not empty */
    if( (stat & KBC_IBF) == 0 ) {
      if (sys_outb(KBC_CMD_REG, cmd)) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 1;
}

int (kbc_write_argument)(uint8_t cmd) {
  uint8_t stat;
  for (int i = 0; i < KBC_RETRY_CNT; i++) {
    if (util_sys_inb(KBC_ST_REG, &stat)) return 1;
    /* loop while 8042 input buffer is not empty */
    if( (stat & KBC_IBF) == 0 ) {
      if (sys_outb(KBC_OUT_BUF, cmd)) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 1;
}

int (kbc_read_return)(uint8_t* cmd) {
  uint8_t stat;
  if (cmd == NULL) return 1;
  for (int i = 0; i < KBC_RETRY_CNT; i++) {
    if (util_sys_inb(KBC_ST_REG, &stat)) return 1;
    /* loop while 8042 output buffer is empty */
    if( stat & KBC_OBF ) {
      if (util_sys_inb(KBC_OUT_BUF, cmd)) return 1;
      return ( (stat &(KBC_PAR_ERR | KBC_TO_ERR)) );
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 1;
}

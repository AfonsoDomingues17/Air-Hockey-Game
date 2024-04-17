#include "i8042.h"

int (kbc_read_cmd)(uint8_t* cmd) { // le um comando do controlador do teclado
  int attempts = KBC_ATTEMPTS;
  if (cmd == NULL) return 1;
  while (attempts--) {
    uint8_t status;
    if (util_sys_inb(KBC_ST_REG, &status) != 0) return 1;
    if (status & KBC_OBF) { // verifica se o buffer de saída está cheio
      if (util_sys_inb(OUT_BUF, cmd) != 0) return 1;
      return ((status & ( KBC_PARITY | KBC_TIMEOUT )));
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 1;
}

int (kbc_write_arg)(uint8_t cmd) { // escrever um argumento no controlador do teclado
  int attempts = KBC_ATTEMPTS;
  while (attempts--) {
    uint8_t status;
    if (util_sys_inb(KBC_ST_REG, &status) != 0) return 1;
    if (!(status & KBC_IBF)) { // verifica se o buffer de entrada não está cheio
      if (sys_outb(IN_BUF, cmd) != 0) return 1; // escreve o argumento no buffer de entrada
      return 0;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 1;
}

int (kbc_write_cmd)(uint8_t cmd) { // escreve comando para o controlador do teclado
  int attempts = KBC_ATTEMPTS;
  while (attempts--) {
    uint8_t status;
    if (util_sys_inb(KBC_ST_REG, &status) != 0) return 1;
    if ((status & KBC_IBF) == 0 ) { // verifica se o buffer de entrada não está cheio
      if (sys_outb(KBC_CMD_REG, cmd) != 0) return 1; // escreve o comando no register de comando
      return 0;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 1;
}

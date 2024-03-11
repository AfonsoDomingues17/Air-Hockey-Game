#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

int hook = KBC_IRQ;
uint8_t scancode = 0;
bool flag = false;

int (kbc_get_status)(uint8_t *status) { // status do controlador do teclado
  if (status == NULL) return 1;
  if (util_sys_inb(KBC_ST_REG, status) != 0) return 1; // le status
  return 0;
}

int (kbc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1; 
  *bit_no = BIT(hook); 
  return sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}

int (kbc_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook);
}

void (kbc_ih)() { // tratamento de interrupção do teclado
    uint8_t status;
    uint8_t data;
    flag = false;
    int attempts = KBC_ATTEMPTS; // número de tentativas para ler o scancode
    while (attempts--) {
        if (kbc_get_status(&status) != 0) continue;
        if (status & KBC_OBF) { // se buffer de saida está cheio
            if (util_sys_inb(OUT_BUF, &data) != 0) continue; // lê o scancode do buffer de saída
            if ((status & (KBC_PARITY | KBC_TIMEOUT)) == 0 ) { // verifica se não houve erro
                scancode = data; // armazena scancode
                flag = false;
            }
            else {
                flag = true;         
            }
            return; // retorna apos ler scancode
        }
        tickdelay(micros_to_ticks(WAIT_KBC)); // atraso para aguardar a disponibilidade do buffer de saída
    }
}

int (read_out_buff)() { // ler o scancode do buffer de saída
  int attempts = KBC_ATTEMPTS;
  int error_code = 0;
  while (attempts--) {
      uint8_t status;
      if (kbc_get_status(&status) != 0) continue;
      if(status & KBC_OBF) {
          if (util_sys_inb(OUT_BUF, &scancode) != 0) continue; // lê o scancode do buffer de saída
          error_code = status & (KBC_PARITY | KBC_TIMEOUT); // obtém o código de erro, se houver
          return error_code;
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

int (kbc_enable_int)() {
  uint8_t commandByte;
  if (kbc_read_cmd_cb(&commandByte) != 0) return 1;
  commandByte |= INT;
  if (kbc_write_cmd_cb(commandByte) != 0) return 1;
  return 0;
}

int (kbc_read_data)(uint8_t *data) {
  uint8_t status;
  while (true) {
    if (util_sys_inb(KBC_ST_REG, &status)) return 1;
    if (status & KBC_OBF) {
      if (status & (KBC_PARITY | KBC_TIMEOUT | KBC_AUX)) return 1;
      return util_sys_inb(OUT_BUF, data);
    }  
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 1;
}


int (kbc_read_cmd_cb)(uint8_t *commandByte) {
  if (kbc_write_cmd_cb(KBC_CMD_READ) != 0) return 1;
  if (util_sys_inb(OUT_BUF, commandByte) != 0) return 1;
  return 0;
}

int (kbc_write_cmd_cb)(uint8_t commandByte) {
  for (int i = 0; i < 3; i++) {
    uint8_t status;
    if (util_sys_inb(KBC_ST_REG, &status) != 0) return 1;
    if (status & (KBC_PARITY | KBC_TIMEOUT | KBC_AUX)) return 1;
    if (status & KBC_INH) {
      tickdelay(micros_to_ticks(WAIT_KBC));
      continue;
    }
    if (sys_outb(KBC_CMD_REG, commandByte)) return 1;
    break;
    }
  if (sys_outb(KBC_CMD_REG, KBC_CMD_WRITE)) return 1;
  if (sys_outb(IN_BUF, commandByte)) return 1;
  return 0;
}

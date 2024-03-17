#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"

extern int count;
uint8_t scancode = 0;
int keyboard_hook_id = IRQ_teclado;
bool deu_erro;

int (subscribe_KBC)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(keyboard_hook_id);

  return sys_irqsetpolicy(IRQ_teclado, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);  
}

int (unsubscribe_KBC)() {
  return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
  uint8_t status, dados;
  deu_erro = false;
  for (int k = 0; k < 10; k++) {
     if (util_sys_inb(KBC_ST_CMD_REG, &status)) {
      continue;
     }
    if (status & KBC_ST_OBF) {
      if (util_sys_inb(KBC_OUT_BUF, &dados)) {
        continue;
      }
      if ((status & (ERRO_TIMEOUT | ERRO_PARIDADE)) == 0) {
        scancode = dados;
      } 
      else {
        deu_erro = true;
        return;
      }
      return;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

int read_OutB() {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(KBC_ST_CMD_REG, &status)) continue;
    if (status & KBC_ST_OBF) {
      if (status & (ERRO_PARIDADE | ERRO_TIMEOUT | ERRO_AUXMOUSE)) continue;
        return util_sys_inb(KBC_OUT_BUF, &scancode);
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int (write_read_command(uint8_t comando)) {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(KBC_ST_CMD_REG, &status)) continue;
    if (!(status & KBC_ST_IBF)) {
      return sys_outb(KBC_ST_CMD_REG, comando);
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int (ask_the_return)(uint8_t *comando) {
  if (comando == NULL) return 1;
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(KBC_ST_CMD_REG, &status)) continue;
    if (status & KBC_ST_OBF) {
      if (status & (ERRO_PARIDADE | ERRO_TIMEOUT)) continue;
      return util_sys_inb(KBC_OUT_BUF, comando);
    }
    tickdelay(micro_delay(DELAY_US));
  }
  return 1;
}

int (write_arguments)(uint8_t comando) {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(KBC_ST_CMD_REG, &status)) continue;
    if (!(status & KBC_ST_IBF)) {
      if (status & (ERRO_PARIDADE | ERRO_TIMEOUT)) continue;
      return sys_outb(0X60, comando);
    }
  }
  return 1;
}

#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"

extern int count;
uint8_t scancode = 0;
int keyboard_hook_id = IRQ_teclado;


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
  for (int k = 0; k < 10; k++) {
      #ifdef LAB3
      count++;
      #endif
     if (util_sys_inb(KBC_ST_REG, &status)) {
      continue;
     }
    if (status & KBC_ST_OBF) {
      #ifdef LAB3
      count++;
      #endif
      if (util_sys_inb(KBC_OUT_BUF, &dados)) {
        continue;
      }
      if ((status & (ERRO_TIMEOUT | ERRO_PARIDADE)) == 0) {
        scancode = dados;
      } 
      else {
        return;
      }
    }
    //tickdelay(micros_to_ticks(DELAY_US));
  }
}

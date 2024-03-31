#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8254_i8042.h"

uint8_t scancode;
bool failed;
int keyboard_count = 0;
int keyboard_hook = 1;

int (util_sys_inb)(int port, uint8_t *valor) {
  if (valor == NULL | port > 2 | port < 0) return 1;
  #ifdef LAB3
  keyboard_count++;
  #endif
  uint32_t temp;
  if (sys_inb(port, &temp)) return 1;
  *valor = (uint8_t) temp;
  return 0;
}

void (kbc_ih)() {
  uint8_t status, dados;
  failed = false;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(READ_STATUS, &status)) continue;

    if (status & OUT_BUFFER_FULL) {
      if (util_sys_inb(READ_WRITE_DATA, &dados)) continue;

      if ((status & (PARITY_ERROR | TIMEOUT_ERROR)) == 0) {
        scancode = dados;
      } 
      else {
        failed = true;
        return;
      }
      return;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

int (keyboard_subscribe)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(keyboard_hook);
  return sys_irqsetpolicy(IRQ_KEYBOARD, (IRQ_REENABLE|IRQ_EXCLUSIVE), &keyboard_hook);
}

int (keyboard_unsubscribe)() {
  return sys_irqrmpolicy(&keyboard_hook);
}

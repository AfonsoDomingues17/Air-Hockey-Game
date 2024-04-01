#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"

#include "i8254_i8042.h"

uint8_t scancode;
bool failed;
int keyboard_count = 0;
int keyboard_hook = 1;

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  #ifdef LAB3
  keyboard_count++;
  #endif
  uint32_t temp;
  if (sys_inb(port, &temp)) return 1; 
  *value = (uint8_t) (temp);

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

int (read_out)() {
  uint8_t status;
  for (int k = 0; k< 10; k++) {
  if (util_sys_inb(READ_STATUS, &status)) continue;
  if (status & OUT_BUFFER_FULL) {
    if ((status & (PARITY_ERROR | TIMEOUT_ERROR | MOUSE_ERROR)) == 0) {
      return util_sys_inb(READ_WRITE_DATA, &scancode);
    }
  }
  }
  return 1;
}

int write_command(uint8_t comando) {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(READ_STATUS, &status)) continue;
    if (!(status & BIT(1))) {
      return sys_outb(READ_STATUS, comando);
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int receive_command(uint8_t *comando) {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(READ_STATUS, &status)) continue;
    if (status & OUT_BUFFER_FULL) {
      if (status & (PARITY_ERROR | MOUSE_ERROR | TIMEOUT_ERROR)) continue;
      return util_sys_inb(READ_WRITE_DATA, comando);
    }
  }
  return 1;
}

int receive_arguments(uint8_t comando) {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(READ_STATUS, &status)) continue;
    if (status & (PARITY_ERROR | MOUSE_ERROR | TIMEOUT_ERROR)) continue;
    return sys_outb(READ_WRITE_DATA, comando);
  }
  return 1;
}

int timer_hook = 0;

int timer_subscribe(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook);
  return sys_irqsetpolicy(0, IRQ_REENABLE, &timer_hook);
}

int timer_unsubscribe() {
  return sys_irqrmpolicy(&timer_hook);
}

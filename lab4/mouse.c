#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "mouse.h"

int hook_mouse = 2;
uint8_t byte;
bool deu_erro = false;
int counter_packet = 0;

int subscribe_mouse(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_mouse);
  return sys_irqsetpolicy(IRQ_MOUSE, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_mouse);
}

int unsubscribe_mouse() {
  return sys_irqrmpolicy(&hook_mouse);
}

void read_packets() {
  uint8_t status, dados;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(LER_STATUS, &status)) continue;
    if (status & KBC_ST_OBF) {
      if (util_sys_inb(KBC_OUT_BUF, &dados)) continue;
      if (dados & (ERRO_AUXMOUSE | ERRO_PARIDADE | ERRO_TIMEOUT)) {
        deu_erro = true;
        return;
      }
      else {
        counter_packet++;
        byte = dados;
      }
    }
    return;
  }
  tickdelay(micros_to_ticks(DELAY_US));
}

int (mouse_enable_data_reporting)() {
    while (true) {
        if (write_mouse_cmd(0xF4)) continue;
        return 0;
    }
}

int (mouse_disable_data_reporting)() {
    while (true) {
        if (write_mouse_cmd(0xF5)) continue;
        return 0;
    }
}

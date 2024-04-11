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

void (mouse_ih)() {
  uint8_t status, dados;
  deu_erro = false;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(LER_STATUS, &status)) continue;

    if (status & KBC_ST_OBF) {
      if (util_sys_inb(KBC_OUT_BUF, &dados)) continue;
      if (status & (ERRO_PARIDADE | ERRO_TIMEOUT)) {
        deu_erro = true;
        return;
      }
      if (status & AUXMOUSE) {
        byte = dados;
      }
      return;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

int (write_mouse_cmd)(uint8_t cmd) {
  if (write_command(Write_Byte_to_Mouse)) return 1;
  if (write_arguments(cmd)) return 1;
  uint8_t temp;
  if (receive_acknowledgment_byte(&temp)) return 1;
  return !(temp == ACK);

}

int (mouse_enable_data_reporting2)() {
    while (true) {
        if (write_mouse_cmd(enable_command)) continue;
        return 0;
    }
}

int (mouse_disable_data_reporting2)() {
    while (true) {
        if (write_mouse_cmd(disable_command)) continue;
        return 0;
    }
}

int write_command(uint8_t comando) {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(LER_STATUS, &status)) continue;

    if ( ! (status & KBC_ST_IBF)) {
      return sys_outb(LER_STATUS, comando);
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int write_arguments(uint8_t comando) {
  uint8_t status;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(LER_STATUS, &status)) continue;
    if (!(status & KBC_ST_IBF)) {
      return sys_outb(KBC_OUT_BUF, comando);
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int receive_acknowledgment_byte(uint8_t *byte) {
  uint8_t status;
  if (byte == NULL) return 1;
  for (int k = 0; k < 10; k++) {
    if (util_sys_inb(LER_STATUS, &status)) continue;

    if (status & (ERRO_PARIDADE | ERRO_TIMEOUT)) return 1;
    if (status & KBC_ST_OBF) {
      return util_sys_inb(KBC_OUT_BUF, byte);
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

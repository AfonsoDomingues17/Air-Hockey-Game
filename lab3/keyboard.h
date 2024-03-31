#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

int (util_sys_inb)(int port, uint8_t *valor);
void (kbc_ih)();
int (keyboard_subscribe)(uint8_t *bit_no);
int (keyboard_unsubscribe)();

#endif

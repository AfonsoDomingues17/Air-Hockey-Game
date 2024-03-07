#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include <lcom/lcf.h>
#include <stdint.h>
#include "KBC.h"

int (subscribe_KBC)(uint8_t *bit_no);

int (unsubscribe_KBC)();

void (kbc_ih)();
int (restore_interrupts)();


#endif /* KEYBOARD_H_ */

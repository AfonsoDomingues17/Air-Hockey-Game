#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <lcom/lcf.h>

int (subscribe_KBC)(uint8_t *bit_no);
int (unsubscribe_KBC)();
void (kbc_ih)();

#endif /* _LCOM_KEYBOARD_H */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <lcom/lcf.h>

int (subscribe_KBC)(uint8_t *bit_no);
int (unsubscribe_KBC)();
void (kbc_ih)();
int (read_OutB)();
int (write_read_command)(uint8_t cmd);
int (write_arguments)(uint8_t comando);
int (ask_the_return)(uint8_t *comando);

#endif /* _LCOM_KEYBOARD_H */

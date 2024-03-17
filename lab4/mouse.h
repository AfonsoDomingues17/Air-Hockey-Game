#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

int (mouse_get_conf)(uint8_t *st);

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

int (write_mouse_cmd)(uint8_t cmd);

int (mouse_enable_reporting)();

int (mouse_disable_reporting)();

int16_t (nine_bit_sign_extension) (uint16_t number);

#endif

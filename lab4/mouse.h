#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_get_status)(uint8_t *status);

void (mouse_ih)();

void (sync_bytes)();

void (bytes_to_packet)();

int (write_mouse_cmd)(uint8_t cmd);

int (mouse_disable_reporting)();

int (mouse_unsubscribe_int)();

int (mouse_enable_reporting)();

#endif

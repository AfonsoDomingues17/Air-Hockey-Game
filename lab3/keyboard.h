#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>

int (kbc_get_status)(uint8_t *status);

int (kbc_subscribe_int)(uint8_t *bit_no);

int (kbc_unsubscribe_int)();

void (kbc_ih)();

int (kbc_write_cmd)(uint8_t cmd);

int (kbc_read_cmd)(uint8_t* cmd);

int (kbc_write_arg)(uint8_t cmd);

int (kbc_enable_int)();

int (kbc_read_data)(uint8_t *data);

int (kbc_read_cmd_cb)(uint8_t *commandByte);

int (kbc_write_cmd_cb)(uint8_t commandByte);

int (kbc_int_handler)();

#endif /* _LCOM_KEYBOARD_H */

#ifndef KBC_FUNCS_H_
#define KBC_FUNCS_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

int (read_out_buffer)(uint8_t* scancode);
int (write_CMD_TO_KBC)(uint8_t port,uint8_t cmd);

#endif /* KBC_FUNCS_H_ */

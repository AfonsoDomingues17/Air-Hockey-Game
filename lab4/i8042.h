#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/lcf.h>

#define KBC_IRQ 1

#define KBC_PARITY BIT(7)
#define KBC_TIMEOUT BIT(6)
#define KBC_AUX BIT(5)
#define KBC_INH BIT(4)
/*
#define KBC_A2 BIT(3)
#define KBC_SYS BIT(2)
*/
#define KBC_IBF BIT(1)
#define KBC_OBF BIT(0)

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
      
#define OUT_BUF 0x60  
#define IN_BUF 0x60  

#define KBC_CMD_READ 0x20
#define KBC_CMD_WRITE 0x60

#define BREAK_ESC 0X81
#define KBC_2BYTE 0xE0
#define KBC_MAKECODE BIT(7)

#define WAIT_KBC 20000
#define KBC_ATTEMPTS 10

#define DIS2 BIT(5)
#define DIS BIT(4)
#define INT2 BIT(1)
#define INT BIT(0)

#define MOUSE_IRQ 12    

#define LB BIT(0) /* state of left button*/
#define RB BIT(1) /* state of right button*/
#define MB BIT(2) /* state of middle button*/
#define FIRST_BYTE BIT(3) 
#define MSB_X_DELTA BIT(4) /* displacement X axis */
#define MSB_Y_DELTA BIT(5) /* displacement Y axis */
#define X_OVFL BIT(6) /* flag if mouse displacement too large */
#define Y_OVFL BIT(7)

#define MOUSE_WRITE 0xD4
#define ENABLE_DR 0xF4
#define DISABLE_DR 0xF5
#define ACK 0xFA

int (kbc_read_cmd)(uint8_t* cmd);

int (kbc_write_arg)(uint8_t cmd);

int (kbc_write_cmd)(uint8_t cmd);

#endif /* _LCOM_I8042_H */

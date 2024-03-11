#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

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

/*
#define DIS2 BIT(5)
#define DIS BIT(4)
#define INT2 BIT(1)
#define INT BIT(0)
*/

#endif /* _LCOM_I8042_H */

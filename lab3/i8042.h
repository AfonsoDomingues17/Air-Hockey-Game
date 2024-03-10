#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ 1 //keyboard interrupt handler line


#define KBC_ST_REG 0x64 //status register
#define KBC_CMD_REG 0x64 //comand register
#define KBC_OUT_BUF 0x60 //output buffer port
#define KBC_IN_BUF 0x60 //input buffer port
#define KBC_READ_CMD 0x20 //read command byte
#define KBC_WRITE_CMD 0x60 //write command byte


#define KBC_PAR_ERR 0x80 //parity error
#define KBC_TO_ERR 0x40 //timeout error
#define KBC_ST_IBF 0x02 //input buffer full
#define KBC_ST_OBF 0x01 //output buffer full
#define KBC_ST_AUX 0x20 //Mouse data

#define TWO_BYTE_SCAN 0xE0 //first by of a two byte scancode
#define BREAK_CODE_ESC 0x81 //break code of ESC key




#define DELAY_US 20000 //program delay

#endif /* _LCOM_I8042_H */

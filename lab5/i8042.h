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


#define MOUSE_IRQ 12 //mouse interrupt line handler
#define MOUSE_LB BIT(0) //Mouse left button pressed
#define MOUSE_RB BIT(1) //Mouse right buttton pressed
#define MOUSE_CB BIT(2) //Mouse center button pressed
#define X_MSB BIT(4) //MSBit of the displacement of the mouse in the X axis
#define Y_MSB BIT(5) //MSBit of the displacement of the mouse in the Y axis
#define X_OVFL BIT(6) //flag that tells if the mouse displacement in the X axis is too large to be represent by a 2's complement 9 byte
#define Y_OVFL BIT(7) //flag that tells if the mouse displacement in the Y axis is too large to be represent by a 2's complement 9 byte
#define X_DELTA_Y_DELTA 0xFFFF
#define MOUSE_CMD 0xD4
#define ENABLE_DATA_REPORTING 0xF4
#define DISABLE_DATA_REPORTING 0xF5
#define ACK 0xFA //Everything is OK
#define NACK 0xFE //Invalid byte
#define ERROR 0xFC //Second consecutive invalid byte



#define DELAY_US 20000 //program delay

#endif /* _LCOM_I8042_H */

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* KEYBOARD*/

#define DELAY_US    20000 // tickdelay(micros_to_ticks(DELAY_US));
#define IRQ_teclado 1

#define BREAK_ESC 0x81 //breakcode tecla ESC
#define BREAK_CODE_BIT BIT(7)  //confirma se é make code ou break code
#define IS_2BYTE_CODE 0xE0 //Verifica se o scan code é de 2 bytes

/*REGISTOS*/
#define LER_STATUS 0x64 //ler status
#define KBC_OUT_BUF 0x60 //ler e escrever dados
#define KBC_OUT_IN 0x60

/*ERROS*/
#define KBC_ST_IBF BIT(1) //INPUT BUFFER CHEIO
#define KBC_ST_OBF BIT(0) //output BUFFER CHEIO
#define ERRO_PARIDADE BIT(7)
#define ERRO_TIMEOUT BIT(6)
#define AUXMOUSE BIT(5)

/*COMANDOS PARA O KBC*/
#define LER_COMANDO_KBC 0x20 //LER UM COMANDO
#define ESCREVER_COMANDO_KBC 0x60 //ESCREVER UM COMANDO
#define enable_interrupt_on_OBF BIT(0) //PEDE AO WRITE_READ QUE NA FUNÇÃO RETURN SEJA PASSADO UM CMD QUE ATIVE OS INTERRUPTS

/*MOUSE*/
#define IRQ_MOUSE 12
#define SYNC_BIT BIT(3)

/*Mouse-Related KBC Commands*/
#define Read_Command_Byte 0x20
#define Write_Command_Byte 0x60

#define ACK 0xFA //if everything OK when enabling the mouse
#define NACK 0xFE //if invalid byte
#define ERROR 0xFC //second consecutive invalid byte

#define enable_command 0xF4
#define disable_command 0xF5

#define Write_Byte_to_Mouse 0xD4 //comando usado quando faço enable do mouse


#endif /* _LCOM_I8042_H */


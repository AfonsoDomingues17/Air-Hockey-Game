#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* KEYBOARD*/

#define DELAY_US    20000 // tickdelay(micros_to_ticks(DELAY_US));
#define IRQ_teclado 1

#define BREAK_ESC 0x81 //breakcode tecla ESC
#define BREAK_CODE_BIT BIT(7)  //confirma se é make code ou break code
#define IS_2BYTE_CODE 0xE0 //Verifica se o scan code é de 2 bytes

/*REGISTOS*/
#define KBC_ST_REG 0x64 //ler status e escrever comandos
#define KBC_CMD_REG 0x64 //ler status e escrever comandos
#define KBC_OUT_BUF 0x60 //ler e escrever dados


/*ERROS*/
#define KBC_ST_OBF BIT(0) //output BUFFER CHEIO //O ERRO NO MEU CÓDIGO ESTAVA AQUI, SIM, EU SOU SUPER FELIZ COM A MINHA VIDA
#define ERRO_PARIDADE BIT(7)
#define ERRO_TIMEOUT BIT(6)

#endif /* _LCOM_I8042_H */


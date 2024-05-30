

#include <lcom/lcf.h>

#include "queue.h"
#include "model/model.h"

#define BASE_ADDR_COM1 0x3F8

#define SPMASK BIT(4) //serial port interrupt mask

#define LCR 0x03 //Allows the setting of the main asynchronous communication parameters: number of bits per character, number of stop bits and parity(define os parametros da comunicaçao)
/*Dar stup á linha de comunicaçao */

#define LSR 0x05 //Shows the status of the communication line (whether a character was transmitted or received, and in the latter case whether an error was detected)
/*mostra o estado da linha*/

#define IER 0x01 //(interrupt enable register) Allows the activation of the different types of interruptions
/* permite a ativaçao de diferentes tipos de interrupçoes */

#define IIR 0x02 //(interrupt information register) Provides information regarding the event that caused an interrupt
/* permite saber informaçao sobre o evento que causou a interrupçao */

#define DLL 0x00 //Divisor Latch Low
/*Allows the setting of the the bit rate (by means of a frequency divider), via the Divisor Latches of the programmable bit-rate generator*/
#define DLM 0x01 //Divisor Latch High


#define FCR 0x02 //FIFO Control Register(Allows the control of FIFO buffering)

#define SP_INTLINE 4

#define RBR 0x00 //receiver buffer register
#define THR 0 //transmitter holding register


#define LSR_REC_DATA BIT(0)
#define LSR_OVERRUN_ERR BIT(1)
#define LSR_PARITY_ERR BIT(2)
#define LSR_FRAME_ERR BIT(3)
#define LSR_BREAK_INT BIT(4)
#define LSR_THR_EMPTY BIT(5) //when UARt is ready to transmit a new character to transmite
#define LSR_TRANS_HOLD_EMPTY BIT(6) //when THR(transmitter holding register) and TSR(transmitter shift register) are empty
#define LSR_FIFO_ERR BIT(7) //when there is an error in the FIFO 

//Bits 0 and 4 are reseted when the LSR is read

#define IER_REC_DATA BIT(0) //enables the reception of data
#define IER_THR_EMPTY BIT(1) //enables the transmission holding register empty interrupt
#define IER_REC_LINE_STATUS BIT(2) //enables the receiver line status interrupt This event is generated when there is a change in the state of bits 1 to 4, i.e.the error bits, of the LSR
#define IER_MODEM_STATUS BIT(3) //enables the modem status interrupt


#define IIR_INT_PEND BIT(0) //if set no interrupt is pending
#define IIR_INT_ID BIT(2) //received data available
#define IIR_RLS BIT(1) | BIT(2) //Receiver Line Status
#define IIR_THR_EMPTY BIT(1) //Transmitter Holding Register Empty
#define IIR_CHAR_TIMEOUT BIT(3) | BIT(2) //Character Timeout
#define IIR_INT_ORIGIN BIT(3) | BIT(2) | BIT(1) // Bits that hold interrupt identification

//need to subscribe UART interrupts in exclusive mode

#define FCR_ENABLE BIT(0) //enables both FIFOs
#define FCR_CLEAR_RECEIVE BIT(1) //clears the contents of the receiver FIFO
#define FCR_CLEAR_XMIT BIT(2) //clears the contents of the transmitter FIFO
#define FCR_ENABLE_64 BIT(5) //enables 64-byte FIFO

/*
UART REGISTERS

RBR register to read the characters received from the serial port
THR register to write characters to be transmitted
LSR register to check the status of the serial port
IER register to enable or disable the different types of interruptions
DDL(Divisor Latch Least) significant byte(LSB of clock divisor for setting the bit-rate)
DLM(Divisor Latch Most) significant byte(MSB of clock divisor for setting the bit-rate)
DL(Divisor Latch)  register is a 16-bit register that contains the divisor for setting the bit-rate of the UART


Both ends must agree on the communication parameters, such as the number of bits per character, the number of stop bits, and the parity bit. These parameters are set by the LCR register
*/

int(sp_subscribe_int)();

int(sp_unsubscribe_int)();

void(sp_ih)();

int(read_char)();

int(send_char)(uint8_t data);

int(serialPort_init)();

int(serialPort_exit)();

int(serialPort_resetFIFO)();

int(MovePucks)(uint8_t val);

int(sendNewPositions)();

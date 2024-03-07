#include <lcom/lcf.h>
#include "keyboard.h"
#include <stdint.h>
#include "i8254.h"
#include "i8042.h"
#include "KBC.h"



int teclado_hook_id;
extern uint8_t scancode;
int (subscribe_KBC)(uint8_t *bit_no) {
    teclado_hook_id = *bit_no; // transform bit_no in a 32 bit identifier
    if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &teclado_hook_id) != 0) return 1; //subscribes to the interrupt policy in exclusive mode
  return 0;
}

int (unsubscribe_KBC)() {
  if(sys_irqrmpolicy(&teclado_hook_id)) return 1; //cancels the notification subscription

  return 0;
}

void (kbc_ih)(){
  read_out_buffer(&scancode);
}

int (restore_interrupts)(){
  uint8_t command_byte;
  if(write_CMD_TO_KBC(KBC_CMD_REG,KBC_READ_CMD) != 0) return 1; //write the command to read the command byte
  if(read_out_buffer(&command_byte) != 0) return 1; //read the value of the comand byte

  command_byte = command_byte | 0x01; //update the command byte to enable interrupt notificatios

  if(write_CMD_TO_KBC(KBC_CMD_REG,KBC_WRITE_CMD) != 0) return 1; //write the command for writing the command byte after being updated
  if(write_CMD_TO_KBC(KBC_IN_BUF,command_byte) != 0) return 1; //write the actual command byte

  return 0;
}

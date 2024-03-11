#include <lcom/lcf.h>
#include "mouse.h"
#include <stdint.h>
#include "i8254.h"
#include "i8042.h"
#include "KBC.h"
int mouse_byte = 0;
int mouse_hook_id;
int (subscribe_MOUSE_interrupts)(uint8_t *bit_no){
    mouse_hook_id = *bit_no;
    if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_EXCLUSIVE | IRQ_REENABLE,&mouse_hook_id) != 0) return 1;
}

int (unsubscribe_MOUSE_interrupts)(){
    if(sys_irqrmpolicy(&mouse_hook_id))
}

void (mouse_ih)(){
    if(read_out_buffer(&mouse_byte) != 0) return 1;

}

int (packet_assembly)(uint8_t packet[]);
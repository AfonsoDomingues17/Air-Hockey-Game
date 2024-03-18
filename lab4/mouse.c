#include <lcom/lcf.h>
#include "mouse.h"
#include <stdint.h>
#include "i8254.h"
#include "i8042.h"
#include "KBC.h"
uint8_t mouse_byte = 0;
int counter_bytes_pack = 0;
extern struct packet packets;

uint8_t packet[3];
int mouse_hook_id;
int (subscribe_MOUSE_interrupts)(uint8_t *bit_no){
    mouse_hook_id = *bit_no;
    if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_EXCLUSIVE | IRQ_REENABLE,&mouse_hook_id) != 0) return 1;
    return 0;
}

int (unsubscribe_MOUSE_interrupts)(){
    if(sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
    return 0;
}

void (mouse_ih)(){
    read_out_buffer(&mouse_byte,1);

}

void (packet_creation)(){
    if((counter_bytes_pack == 0) && (mouse_byte & BIT(3))){
        packet[counter_bytes_pack++] = mouse_byte;
    }
    else if(counter_bytes_pack == 2){
        packet[counter_bytes_pack++] = mouse_byte;
    }
    else if (counter_bytes_pack == 1){
        packet[counter_bytes_pack++] = mouse_byte;
    }

}

void (packets_assembly)(){
    packets.bytes[0] = packet[0];
    packets.bytes[1] = packet[1];
    packets.bytes[2] = packet[2];

    packets.lb = (packet[0] & MOUSE_LB);
    packets.rb = (packet[0] & MOUSE_RB);
    packets.mb = (packet[0] & MOUSE_CB);

    packets.x_ov = (packet[0] & X_OVFL);
    packets.y_ov = (packet[0] & Y_OVFL);
    if(packet[0] & X_MSB){
        packets.delta_x = (packet[1] | 0xFF00); //if MSB is 1 i need to extended the  signal
    }
    else packets.delta_x = packet[1];//if not it will automatically extend with 0

    if(packet[0] & Y_MSB){
        packets.delta_y = (packet[2] | 0xFF00); //if MSB is 1 i need to extended the  signal

    }
    else packets.delta_y = packet[2]; //if not it will automatically extend with 0
}

int (disable_enable_data_reporting)(uint8_t cmd){
    uint8_t mouse_response;
    int tentativas = 10;
    while(tentativas > 0){
    if(write_CMD_TO_KBC(KBC_CMD_REG,MOUSE_CMD) != 0) return 1; //write byte to the mouse
    if(write_CMD_TO_KBC(KBC_IN_BUF,cmd) != 0) return 1; //write the aurguments of the mouse command to the input buffer
    if(read_out_buffer(&mouse_response,1) != 0) return 1;
    if(mouse_response == ACK) return 0;
    tentativas--;
    }
    return 1;
}





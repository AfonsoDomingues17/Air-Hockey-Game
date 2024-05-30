#include <lcom/lcf.h>
#include "serialport.h"

Queue *inQueue;

extern Sprite* redpuck;
extern Sprite* bluepuck;
extern Sprite* Ball;

int sp_hook_id = 4;

int(sp_subscribe_int)(){
    return sys_irqsetpolicy(SP_INTLINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &sp_hook_id);
}

int(sp_unsubscribe_int)(){
    return sys_irqrmpolicy(&sp_hook_id);
}

void(sp_ih)(){
    uint8_t iir;
    if(util_sys_inb(BASE_ADDR_COM1 + IIR, &iir) != 0){
        return;
    }
    if((iir & IIR_INT_PEND) == 0){ //check if interrupt is pending
        if(iir & IIR_INT_ID){ //check if the interrupt corresnponds to data available
            while(!read_char());
        }
    }
}

int(read_char)(){
    uint8_t status;
    uint8_t data;
    if(util_sys_inb(BASE_ADDR_COM1 + LSR, &status) != 0){
        printf("status read failled\n");
        return 1;
    }
    if(status & LSR_REC_DATA){
        if(util_sys_inb(BASE_ADDR_COM1 + RBR, &data) == 0){
            if(status & (LSR_OVERRUN_ERR | LSR_PARITY_ERR | LSR_FRAME_ERR)){
                printf("Ardeu"); //check if there are no errors
                return 1;
            }
            enqueue(inQueue, data);
            return 0;
            //dar push do character para a fila porque os dados nao podem ser processados imediatamente
            //the interrupt handler just collects the data and stores it for later processing
        }  
    }
    return 1;
}

int(send_char)(uint8_t data){
    uint8_t status;
    int attempts = 10;
    while(attempts > 0){
        if(util_sys_inb(BASE_ADDR_COM1 + LSR, &status) != 0){
            return 1;
        }
        if(status & (LSR_OVERRUN_ERR | LSR_PARITY_ERR | LSR_FRAME_ERR)){ //check if there are no errors
            return 1;
        }
        if(status & (LSR_THR_EMPTY | BIT(6))){ //check if the transmitter holding register is empty            
            if(sys_outb(BASE_ADDR_COM1 + THR, data) != 0){
                return 1;
            }
            return 0;
        }
        attempts--;
    }
    return 1;
}

int(serialPort_init)(){
    uint8_t ier; //interrupt enable register
    if(util_sys_inb(BASE_ADDR_COM1 + IER, &ier) != 0){
        return 1;
    }
    ier = ier | IER_REC_DATA; //enable the reception of data
    if(sys_outb(BASE_ADDR_COM1 + IER, ier) != 0){ //update the interrupt enable register
        return 1;
    }
    inQueue = create_queue();
    return 0;
}

int(serialPort_exit)(){
    destroy_queue(inQueue);
    return 0;
}

int (serialPort_resetFIFO)(){
    if(sys_outb(BASE_ADDR_COM1 + FCR,FCR_ENABLE | FCR_CLEAR_RECEIVE | FCR_CLEAR_XMIT)) return 1;
    while(!is_empty(inQueue)) {
        dequeue(inQueue);
    }
    return 0;
}

void(transmit_puck_change)(Sprite* bluepuck, int* previous_x, int* previous_y) {
    if (bluepuck == NULL || (bluepuck->x == *previous_x && bluepuck->y == *previous_y )) return;
    int16_t delta_x = bluepuck->x - *previous_x;
    int16_t delta_y = bluepuck->y - *previous_y;
    send_char(delta_x & 0x00FF);
    tickdelay(1);
    send_char(delta_x >> 8);
    tickdelay(1);
    send_char(delta_y & 0x00FF);
    tickdelay(1);
    send_char(delta_y >> 8);
    *previous_x = bluepuck->x;
    *previous_y = bluepuck->y;
}


#include "KBC.h"

extern int counter;
int (read_out_buffer)(uint8_t *byte){
  uint8_t status;
  uint8_t tentativas = 10; //numero razoavel de tentativas
  while(tentativas){

    if(util_sys_inb(KBC_ST_REG,&status) != 0) return 1; //read the status register
    #ifdef LAB3
    counter++; //used to increment the counter of number of sys_inb() calls
    #endif

    if((status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0){ //see if there are no timeout nor parity error
      if (status & KBC_ST_OBF && ((status & KBC_ST_AUX) == 0)){ //check if output_buffer is full and mouse has no data

        if(util_sys_inb(KBC_OUT_BUF,byte) != 0)return 1; //read the scancode from the output buffer
        #ifdef LAB3
        counter++;
        #endif
        return 0;
      }
      if (status & KBC_ST_OBF && (status & KBC_ST_AUX)){ //check if output_buffer is full and mouse has data
        if(util_sys_inb(KBC_OUT_BUF,byte) != 0)return 1; //read the scancode from the output buffer
        #ifdef LAB3
        counter++;
        #endif
        return 0;
      }
      

    }
    
    tentativas--;
    tickdelay(micros_to_ticks(DELAY_US)); //by delay it allows time for the KBD to send a scancode to the output buffer of the KBC
  }
  return 1;

}
int (write_CMD_TO_KBC)(uint8_t port,uint8_t cmd){
  uint8_t status;
  uint8_t tentativas = 10; //numero razoavel de tentativas
  while(tentativas){
    if(util_sys_inb(KBC_ST_REG,&status) != 0) return 1; //read the status register
    #ifdef LAB3
    counter++;
    #endif

    if((status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0){ //see if there are no timeout nor parity error
      if ((status & KBC_ST_IBF) == 0){ //check if input_buffer is full- if not

        if(sys_outb(port,cmd) != 0)return 1; //write commands to the command port
        return 0;
      }
    }
    
    tentativas--;
    tickdelay(micros_to_ticks(DELAY_US)); //by delay it allows time for the KBC to free the input_buffer
  }
  return 1;

}




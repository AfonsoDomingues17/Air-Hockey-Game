#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8254.h"
int counter = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  
  uint8_t cresult;
  if(timer_get_conf(timer,&cresult) != 0) return 1; 
  if(timer_display_conf(timer,cresult,field) != 0 ) return 1;
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if(timer > 2 || timer < 0 || freq > TIMER_FREQ) return 1;
  if(timer_set_frequency(timer,freq) != 0) return 1;

  return 0;
}

int(timer_test_int)(uint8_t time) {
  uint8_t bit_no = 1; //identificador do interrupt
  if(timer_subscribe_int(&bit_no) != 0) return 1;
  int r;
  int ipc_status;
  uint32_t irq_set = BIT(bit_no); //usado para verificar se o interrupt está pending
 message msg;
 while(time>0) { 
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 			
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt
                      timer_int_handler(); //conta o n de interrupçoes que depois nos pode dar o tempo que passou
                      if((counter%60)==0){ //counter represents the elapsed time
                      //timer estã programado para gerar um interrupt a cada 1/60s,logo quando o conter for 60 significa que ja passou um segundo
                        timer_print_elapsed_time(); //print the time that has passed
                        time--;
                      }
                  }
                  break;
              default:
                  break; 
          }
      } else { 
          
      }
   }
  if(timer_unsubscribe_int() != 0) return 1;
  return 0;
}

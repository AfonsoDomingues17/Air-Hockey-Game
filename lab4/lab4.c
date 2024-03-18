// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <mouse.h>
#include <lcom/lab4.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "i8254.h"
#include "i8042.h"

extern int counter_bytes_pack;
int counter_timer = 0;

// Any header files included below this line should have been created by you
struct packet packets;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  uint8_t mouse_hook_id = 3; //mouse interrupt handler line
  int r;
  int irq_set = BIT(mouse_hook_id);
  int ipc_status;
  uint32_t number_packets = 0;
  
  if(subscribe_MOUSE_interrupts(&mouse_hook_id)) return 1; //subscribe to mouse interrupts
  //if(mouse_enable_data_reporting() != 0 ) return 1; //set default to remote mode, change it to stream mode

  if(disable_enable_data_reporting(ENABLE_DATA_REPORTING) != 0 ) return 1; //set default to remote mode, change it to stream mode
  message msg;
  
  while(number_packets < cnt) { //it ends when the number of packets are read
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
        
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 
              
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt
                    mouse_ih(); //mouse interrupt handler
                    packet_creation(); //creates the mouse 3 byte packet
                    if(counter_bytes_pack == 3){ //when it reads the 3 bytes
                      counter_bytes_pack = 0; //counter is reseted
                      packets_assembly(); //packet structure is assembled 
                      mouse_print_packet(&packets); //used to print the packets 
                      number_packets++;
                      
                    }
                  }
                  break;
              default:
                  break; 
          }
      } else { 
          
      }

   }
   if(disable_enable_data_reporting(DISABLE_DATA_REPORTING) != 0) return 1; //disable data_reporting
   if(unsubscribe_MOUSE_interrupts() != 0) return 1; //unsubscribe mouse interrupts
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t mouse_hook_id = 3; //mouse interrupt handler line
  uint8_t timer_hook_id = 0;
  int r;
  int irq_set = BIT(mouse_hook_id);
  int irq_set_timer = BIT(timer_hook_id);
  int ipc_status;
  uint8_t actual_time = idle_time;
  if(timer_subscribe_int(&timer_hook_id) != 0) return 1;
  if(subscribe_MOUSE_interrupts(&mouse_hook_id)) return 1; //subscribe to mouse interrupts
  //if(mouse_enable_data_reporting() != 0 ) return 1; //set default to remote mode, change it to stream mode

  if(disable_enable_data_reporting(ENABLE_DATA_REPORTING) != 0 ) return 1; //set default to remote mode, change it to stream mode
  message msg;
  
  while(actual_time > 0) { //it ends when the number of packets are read
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
        
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 
              
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt

                    mouse_ih(); //mouse interrupt handler
                    packet_creation(); //creates the mouse 3 byte packet
                    if(counter_bytes_pack == 3){ //when it reads the 3 bytes
                      counter_bytes_pack = 0; //counter is reseted
                      packets_assembly(); //packet structure is assembled 
                      mouse_print_packet(&packets); //used to print the packets 
                      
                  
                    }
                    counter_timer = 0;
                    actual_time = idle_time;
                  }
                  if(msg.m_notify.interrupts & irq_set_timer){
                    timer_int_handler();
                    if(counter_timer % 60 == 0){
                        actual_time--;
                    }

                  }
                  break;
              default:
                  break; 
          }
      } else { 
          
      }

   }
   if(disable_enable_data_reporting(DISABLE_DATA_REPORTING) != 0) return 1; //disable data_reporting
   if(unsubscribe_MOUSE_interrupts() != 0) return 1; //unsubscribe mouse interrupts
   if(timer_unsubscribe_int() != 0) return 1;
  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}


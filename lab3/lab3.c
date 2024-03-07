#include <lcom/lcf.h>
#include <keyboard.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8254.h"
#include "i8042.h"

uint8_t scancode = 0;
extern int counter;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  
  uint8_t teclado_hook_id = 1;
  int irq_set = BIT(teclado_hook_id); //which handler is activated
  int r;
  int ipc_status;
  if(subscribe_KBC(&teclado_hook_id)) return 1;
  message msg;
  
  while(scancode != BREAK_CODE_ESC) { 
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 			
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt
                    kbc_ih(); //reads the scancodes
                    
                    kbd_print_scancode(!(scancode & 0x80),scancode == TWO_BYTE_SCAN ? 2 : 1, &scancode);  //see if it is a make code, if the scancode is a two byte size, i am going to compare its MSB with the default value 0xE0 and use the scancode as an array
                  }
                  break;
              default:
                  break; 
          }
      } else { 
          
      }

   }
  if(unsubscribe_KBC() != 0) return 1;
  if(kbd_print_no_sysinb(counter) != 0) return 1;

  return 0;
}

int(kbd_test_poll)() {
  //1º desativar interrupts - feito automaticamente
  //2º polling for scancodes
  //3ºenable interrupts with command byte
  //4º restore command byte
  while(scancode != BREAK_CODE_ESC){
    if(read_out_buffer(&scancode) == 0){
      kbd_print_scancode(!(scancode & 0x80),scancode == TWO_BYTE_SCAN ? 2 : 1, &scancode);
    }
  }
  if(restore_interrupts() != 0) return 1;

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {

  printf("%s is not yet implemented!\n", __func__);

  return 1;
}




#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"
#include <stdbool.h>

extern int count;
extern uint8_t scancode;

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
  int r;
  uint8_t size;

  message msg;
  int ipc_status;

uint8_t bit_no;
if (subscribe_KBC(&bit_no)) return 1;
  while (scancode != BREAK_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) 
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & bit_no) {
          kbc_ih();
          
          size = 1;
          if (scancode == IS_2BYTE_CODE) size = 2; //BTW PORQUE OS MEUS COLEGAS FIZERAM BYTE[2]???


          kbd_print_scancode(!(scancode & BREAK_CODE_BIT), size, &scancode);
        }   
        break;
      
      default:
        break;
      }
    }
  }

  if (kbd_print_no_sysinb(count)) return 1;
  if (unsubscribe_KBC()) return 1;
  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

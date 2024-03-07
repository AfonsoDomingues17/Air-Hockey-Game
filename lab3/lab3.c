#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

extern uint8_t scancode;
extern uint32_t cnt;
extern bool error;

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
  // Subscribe to keyboard interrupts
  int ipc_status, r;
  message msg;
  uint8_t hook;
  bool twoByteCode = false;

  if (keyboard_subscribe_int(&hook)) return 1;

  while (scancode != ESC_BREAK_CODE) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */                
          if (msg.m_notify.interrupts & hook) { /* subscribed interrupt */
            kbc_ih();
            uint8_t size;
            uint8_t bytes[2];      
            if (scancode == TWO_BYTE_SCANCODE) {
              bytes[0] = scancode;
              twoByteCode = true;
              break;
            }
            bool make = !(scancode & BREAK_CODE_BIT);
            if (!twoByteCode) size = 1;
            if (twoByteCode) {
              size = 2;
              bytes[1] = scancode;
              twoByteCode = false;
            } else bytes[0] = scancode;
            kbd_print_scancode(make, size, bytes);
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */    
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (keyboard_unsubscribe_int()) return 1;

  if (kbd_print_no_sysinb(cnt)) return 1;

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

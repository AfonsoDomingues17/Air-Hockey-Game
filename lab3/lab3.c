#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

extern uint8_t scancode;
extern uint32_t sys_inb_cnt;
extern uint32_t timer_cnt;
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
            if (error) break;
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

  if (kbd_print_no_sysinb(sys_inb_cnt)) return 1;

  return 0;
}

int(kbd_test_poll)() {
  bool twoByteCode = false;
  uint8_t stat;
  while (scancode != ESC_BREAK_CODE) {
    if (keyboard_get_conf(&stat)) continue;
    if ((stat & KBC_OBF) == 0 || (stat & KBC_AUX)) continue;
    if (read_out_buffer()) return 1;
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

  //Read Command Byte
  if (kbc_write_command(KBC_READ_CMD)) return 1; 
  uint8_t cmd;
  if (kbc_read_return(&cmd)) return 1; 
  
  //Enable interrupts
  cmd |= KBC_INT_ENBL;
  if (kbc_write_command(KBC_WRITE_CMD)) return 1;
  if (kbc_write_arguments(cmd)) return 1;

  if (kbd_print_no_sysinb(sys_inb_cnt)) return 1;

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
// Subscribe to keyboard interrupts
  int ipc_status, r;
  message msg;
  uint8_t kbc_hook, timer_hook;
  bool twoByteCode = false, idle = false;
  uint32_t last_timed_key_was_pressed = 0;

  if (keyboard_subscribe_int(&kbc_hook)) return 1;
  if (timer_subscribe_int(&timer_hook)) return 1;

  while (scancode != ESC_BREAK_CODE && !idle) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */                
          if (msg.m_notify.interrupts & kbc_hook) { /* subscribed interrupt */
            last_timed_key_was_pressed = timer_cnt;
            kbc_ih();
            if (error) break;
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
          if (msg.m_notify.interrupts & timer_hook) { /* subscribed interrupt */
            timer_int_handler();
            if ((timer_cnt - last_timed_key_was_pressed) == 60 * n) {
              idle = true;
            }
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
  if (timer_unsubscribe_int()) return 1;

  if (kbd_print_no_sysinb(sys_inb_cnt)) return 1;

  return 0;
}

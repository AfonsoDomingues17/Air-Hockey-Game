#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "keyboard.h"
#include "i8254_i8042.h"
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>


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

extern uint8_t scancode;
extern bool failed;
extern int keyboard_count;
int timer_count = 0;

int(kbd_test_scan)() {
  int r, ipc_status;
  message msg;
  uint8_t bit_no;
  uint8_t size;

  if (keyboard_subscribe(&bit_no)) return 1;

  uint8_t byte[2];
  bool is_2byte = false;
  
  while (scancode != ESC) {
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
          if (failed) continue;

          if (scancode == IS_2BYTECODE) {
            byte[0] = scancode;
            is_2byte = true;
            break;
          }

          if (is_2byte) {
            size = 2;
            is_2byte = false;
            byte[1] = scancode;
          }
          else {
            byte[0] = scancode;
            size = 1;
          }

          kbd_print_scancode(!(scancode & IS_BREAKCODE), size, byte);
        }
        break;
      
      default:
        break;
      }
    }
  }

  if (kbd_print_no_sysinb(keyboard_count)) return 1;
  if (keyboard_unsubscribe()) return 1;
  return 0;
}

int(kbd_test_poll)() {
  uint8_t bytes[2];
  uint8_t size;

  bool is_2byte = false;
  
  while (scancode != ESC) {
    if (read_out()) continue;

    if (scancode == IS_2BYTECODE) {
      bytes[0] = scancode;
      is_2byte = true;
      break;
    }
    if (is_2byte) {
      size = 2;
      is_2byte = false;
      bytes[1] = scancode;
    }
    else {
      bytes[0] = scancode;
      size = 1;
    }
    kbd_print_scancode(!(scancode & IS_BREAKCODE), size, bytes);
  }

  uint8_t comando;
  if (write_command(WANNA_USE_COMMAND)) return 1;
  if (receive_command(&comando)) return 1;
  comando = comando | BIT(0);
  if (write_command(0x60)) return 1;
  if (receive_arguments(comando)) return 1;
  return 0;
}


int(kbd_test_timed_scan)(uint8_t n) {
    int r, ipc_status;
  message msg;
  uint8_t bit_no,  timer_no;
  uint8_t size;

  if (keyboard_subscribe(&bit_no)) return 1;
  if (timer_subscribe(&timer_no)) return 1;

  uint8_t byte[2];
  bool is_2byte = false;
  int temp = 0;
  
  while ((scancode != ESC) & (n >= temp)) {
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
          if (failed) continue;

          if (scancode == IS_2BYTECODE) {
            byte[0] = scancode;
            is_2byte = true;
            break;
          }

          if (is_2byte) {
            size = 2;
            is_2byte = false;
            byte[1] = scancode;
          }
          else {
            byte[0] = scancode;
            size = 1;
          }

          kbd_print_scancode(!(scancode & IS_BREAKCODE), size, byte);
          temp = 0;
        }

        if (msg.m_notify.interrupts & timer_no) {
          timer_count++;
          if (timer_count % 60 == 0) {
            temp++;
          }
        }
        break;
      
      default:
        break;
      }
    }
  }

  if (kbd_print_no_sysinb(keyboard_count)) return 1;
  if (keyboard_unsubscribe()) return 1;
  if (timer_unsubscribe()) return 1;

  return 0;
}

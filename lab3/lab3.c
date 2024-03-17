#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "keyboard.h"
#include "i8042.h"

#include "i8254.h"
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

extern int count;
extern uint8_t scancode;
extern bool deu_erro;

int(kbd_test_scan)() {
  int r;
  uint8_t size;

  message msg;
  int ipc_status;

  uint8_t bit_no;
if (subscribe_KBC(&bit_no)) return 1;

  uint8_t bytes[2];
  bool is_2byte_scancode = false;

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
          if (deu_erro) break;

          if (scancode == IS_2BYTE_CODE) {
            bytes[0] = scancode;
            is_2byte_scancode = true;
            break;
          }

          if (is_2byte_scancode) {
            bytes[1] = scancode;
            is_2byte_scancode = false;
            size = 2;
          }
          else {
            bytes[0] = scancode;
            size = 1;
          }

          kbd_print_scancode(!(scancode & BREAK_CODE_BIT), size, bytes);
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
  uint8_t bytes[2];
  uint8_t size;

  bool is_2BYTE = false;

  while (scancode != BREAK_ESC) {
    if (read_OutB()) continue;

    if (scancode == IS_2BYTE_CODE) {
      bytes[0] = scancode;
      is_2BYTE = true;
      break;
    }

    if (is_2BYTE) {
      size = 2;
      is_2BYTE = false;
      bytes[1] = scancode;
    }
    else {
      size = 1;
      bytes[0] = scancode;
    }

    kbd_print_scancode(!(scancode & BREAK_CODE_BIT), size, bytes);
  }

  uint8_t comando;
  if (write_read_command(LER_COMANDO_KBC)) return 1; //AVISA QUE QUER LER UM COMANDO COM O 0X20
  if (ask_the_return(&comando)) return 1; //RECEBE O COMANDO PARA ALTERARMOS
  comando = comando | enable_interrupt_on_OBF; // ALTERA
  if (write_read_command(ESCREVER_COMANDO_KBC)) return 1; //AVISA QUE VAI ESCREVER COM O 0X60
  if (write_arguments(comando)) return 1; //PASSA OS ARGUMENTOS

  if (kbd_print_no_sysinb(count)) return 1;

  return 1;
}

extern int counter_time;

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t size;
  uint8_t bytes[2];
  bool is_2byte_scancode = false;

  message msg;
  int ipc_status;
  int r;

  uint8_t bit_no;
  if (subscribe_KBC(&bit_no)) return 1;
  uint8_t st;
  if (timer_subscribe_int(&st)) return 1;

  while ((scancode != BREAK_ESC) && (counter_time/60 != n)) {
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
          if (deu_erro) break;

          if (scancode == IS_2BYTE_CODE) {
            bytes[0] = scancode;
            is_2byte_scancode = true;
            break;
          }

          if (is_2byte_scancode) {
            bytes[1] = scancode;
            is_2byte_scancode = false;
            size = 2;
          }
          else {
            bytes[0] = scancode;
            size = 1;
          }

          kbd_print_scancode(!(scancode & BREAK_CODE_BIT), size, bytes);
          counter_time = 0;
        }
        if (msg.m_notify.interrupts & st) {
          timer_int_handler();   
        }
        break;
      
      default:
        break;
      }
    }
  }

  if (kbd_print_no_sysinb(count)) return 1;
  if (unsubscribe_KBC()) return 1;
  if (timer_unsubscribe_int()) return 1;
  return 0;
}

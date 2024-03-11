#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include <keyboard.h>

extern uint8_t scancode;
extern int counter;
uint8_t size = 0;
uint8_t bytes[2];

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

/*
    print do make/breakcode
    termina quando lê breakcode do ESC (0X81)
    primeiro byte em ambos scancodes é normalmente 0xE0

    subscrever os interrupts do KBC
        - receber input -> ler scancode do OUT_BUF
        - não precisaamos de configurar o KBC -> inicializado no MINIX

        MINIX tem um IH instalado -> tem de ser desativado para que leia o handler e só depois o OUT_BUF
        --> Usar IRQ_REENABLE | IRQ_EXCLUSIVE

    comandos -> 0x64
    argumentos -> 0x60
    OUT_BUF -> 0x60
    IN_BUF -> 0x64 (comandos) ou 0x60 (argumentos)
*/

int(kbd_test_scan)() {
  uint8_t hook;
  int ipc_status;
  message msg;
  uint8_t irq_set = BIT(KBC_IRQ);
  bool flag = false; // se processo deve continuar ou não
  int r;
  if (kbc_subscribe_int(&hook)) return 1;
  while (!flag) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih(); // interrupcao teclado
            if (scancode == KBC_2BYTE) { // se o scancode é que tipo de byte
              bytes[0] = scancode;
              size++;
            } else {
                bool make;
                bytes[size] = scancode;
                size++;
                if ((KBC_MAKECODE & scancode) == KBC_MAKECODE) { // tipo do código da tecla
                    make = false;
                } else {
                    make = true;
                }
                kbd_print_scancode(make, size, bytes); 
                size = 0; // renicia tamanho do buffer
            }
            if (bytes[0] == BREAK_ESC) flag = true; // sai do loop
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      } 
    } else {  /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
    }
	if (kbc_unsubscribe_int() != 0) return 1; // desubscreve interrupções do teclado
	if (kbd_print_no_sysinb(counter) != 0) return 1; // imprime numero de chamadas sys_inb
	return 0;
}

int(kbd_test_poll)() {
  bool flag = false;
  while (!flag) {
    if (kbc_read_data(&scancode)) return 1;
    if (scancode == KBC_2BYTE) {
      bytes[0] = scancode;
      size++;
    } else {
      bool make;
      bytes[size] = scancode;
      size++;
      if ((KBC_MAKECODE & scancode) == KBC_MAKECODE) { // tipo do código da tecla
        make = false;
      } else {
        make = true;
      }
      kbd_print_scancode(make, size, bytes); 
      size = 0; // renicia tamanho do buffer
    }
    if (bytes[0] == BREAK_ESC) flag = true; // sai do loop
  }
  if (kbc_enable_int() != 0) return 1;
  if (kbd_print_no_sysinb(counter) != 0) return 1; 
	return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

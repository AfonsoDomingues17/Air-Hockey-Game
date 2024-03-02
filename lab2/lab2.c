#include "i8254.h"
#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("PT-PT");

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
  if (timer < 0 || timer > 2) return 1;
  uint8_t config;
  if (timer_get_conf(timer, &config) != 0) return 1;
  if (timer_display_conf(timer, config, field) != 0) return 1;
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2) return 1;
  if (freq > TIMER_FREQ) return 1;
  if (timer_set_frequency(timer, freq) != 0) return 1;
  return 0;
}

int(timer_test_int)(uint8_t time) {
  uint8_t bit_no;
  if (timer_subscribe_int(&bit_no) != 0) return 1;

  int ipc_status;
  message msg;
  int r;

  while (time > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {  //usa uma função maluca chamada driver_receive, provavelmente para receber a mensagem e o status
    // o int r é apenas para ver se a função deu errado ou não
    // ipc = "Inter-Process Communication" permite que processos distintos comuniquem entre si
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { //usa a função is_ipc_notify para ver se o status está ativo
      switch (_ENDPOINT_P(msg.m_source)) //usa um define maluco _ENDPOINT_P, não faço ideia praq
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & bit_no) { //verifica se tem uma nova notificação interrupt e se o timer ainda está subscrito
          timer_int_handler();
          if (counter % 60 == 0) {
            time--;
            timer_print_elapsed_time();
          }
        }   
        break;
      
      default:
        break;
      }
    }
  }

  if (timer_unsubscribe_int() != 0) return 1;

  return 0;
}

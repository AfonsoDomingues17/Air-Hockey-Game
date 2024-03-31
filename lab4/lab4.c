// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"
#include "i8042.h"

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

extern bool deu_erro;
extern uint8_t byte;
extern int counter_packet;
bool extend_1_X_Delta = false;
bool extend_1_Y_Delta = false;
struct packet *caixa;

int (mouse_test_packet)(uint32_t cnt) {
  message msg;
  int r;
  int ipc_status;

  uint8_t hook_mouse;
  if (mouse_enable_data_reporting()) return 1;
  if (subscribe_mouse(&hook_mouse)) return 1;
  
    while (cnt > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) 
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & hook_mouse) {
          read_packets();
          if (deu_erro) continue;
          switch (counter_packet)
          {
          case 1:
            caixa->lb = byte & BIT(0);
            caixa->rb = byte & BIT(1);
            caixa->mb = byte & BIT(2);
            caixa->x_ov = byte & BIT(6);
            caixa->y_ov = byte & BIT(7);

            if ((byte & BIT(4)) != 0) extend_1_X_Delta = true;
            if ((byte & BIT(5)) != 0) extend_1_Y_Delta = true;

            break;
          case 2:
            if (extend_1_X_Delta) {
              extend_1_X_Delta = false;
              uint16_t temp = BIT(8) + byte;
              caixa->delta_x = (temp | 0xFF00);
            }
            else {
            caixa->delta_x = (uint16_t) byte;
            }

            break;
          case 3:
            cnt--;

            if (extend_1_Y_Delta) {
              extend_1_Y_Delta = false;
              uint16_t temp = BIT(8) + byte;
              caixa->delta_x = (temp | 0xFF00);
            }
            else {
            caixa->delta_x = (uint16_t) byte;
            }

            caixa->delta_y = byte;
            mouse_print_packet(caixa);
            break;
          default:
            break;
          }
        }   
        break;
      
      default:
        break;
      }
    }
  }
  if (mouse_disable_data_reporting()) return 1;
  if (unsubscribe_mouse()) return 1;
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
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

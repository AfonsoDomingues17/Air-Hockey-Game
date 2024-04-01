// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "mouse.h"

extern uint8_t byte;
extern bool error;
extern unsigned int time_count;

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
  int ipc_status, r;
  message msg;
  uint8_t hook;

  // Packet variables
  uint8_t packet[3];
  int count = 0;

  if (mouse_enable_reporting()) return 1;

  if (mouse_subscribe_int(&hook)) return 1;
  
  while (cnt) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */              
          if (msg.m_notify.interrupts & hook) { /* subscribed interrupt */
            mouse_ih();
            if (error) continue;
            if (count == 0 && (byte & SYNC_BIT) == 0) continue;
            packet[count++] = byte;
            if (count == 3) {
              count = 0;
              struct packet parsing;
              for (int i = 0; i < 3; i++) {
                parsing.bytes[i] = packet[i];
              }
              parsing.rb = packet[0] & RMB_BIT;
              parsing.mb = packet[0] & MMB_BIT;
              parsing.lb = packet[0] & LMB_BIT;
              parsing.delta_x = packet[1];
              if (packet[0] & MSB_X_DELTA) {
                uint16_t temp = BIT(8) + packet[1];
                parsing.delta_x = nine_bit_sign_extension(temp);
              }
              parsing.delta_y = packet[2];
              if (packet[0] & MSB_Y_DELTA) {
                uint16_t temp = BIT(8) + packet[2];
                parsing.delta_y = nine_bit_sign_extension(temp);
              }
              //parsing.delta_x = ((packet[0] & MSB_X_DELTA) << 4) + packet[1];
              //parsing.delta_y = ((packet[0] & MSB_Y_DELTA) << 3) + packet[2];
              parsing.x_ov = packet[0] & X_OVFL;
              parsing.y_ov = packet[0] & Y_OVFL;
              mouse_print_packet(&parsing);
              cnt--;
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

  if (mouse_unsubscribe_int()) return 1;

  if (mouse_disable_reporting()) return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
int ipc_status, r;
  message msg;
  uint8_t mouse_hook, timer_hook;
  bool done = false;

  // Packet variables
  uint8_t packet[3];
  unsigned int count = 0, idle = 0;

  if (mouse_enable_reporting()) return 1;

  if (mouse_subscribe_int(&mouse_hook)) return 1;
  if (timer_subscribe_int(&timer_hook)) return 1;
  
  while (!done) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */              
          if (msg.m_notify.interrupts & mouse_hook) { /* subscribed interrupt */
            mouse_ih();
            idle = time_count;
            if (error) continue;
            if (count == 0 && (byte & SYNC_BIT) == 0) continue;
            packet[count++] = byte;
            if (count == 3) {
              count = 0;
              struct packet parsing;
              for (int i = 0; i < 3; i++) {
                parsing.bytes[i] = packet[i];
              }
              parsing.rb = packet[0] & RMB_BIT;
              parsing.mb = packet[0] & MMB_BIT;
              parsing.lb = packet[0] & LMB_BIT;
              parsing.delta_x = packet[1];
              if (packet[0] & MSB_X_DELTA) {
                uint16_t temp = BIT(8) + packet[1];
                parsing.delta_x = nine_bit_sign_extension(temp);
              }
              parsing.delta_y = packet[2];
              if (packet[0] & MSB_Y_DELTA) {
                uint16_t temp = BIT(8) + packet[2];
                parsing.delta_y = nine_bit_sign_extension(temp);
              }
              //parsing.delta_x = ((packet[0] & MSB_X_DELTA) << 4) + packet[1];
              //parsing.delta_y = ((packet[0] & MSB_Y_DELTA) << 3) + packet[2];
              parsing.x_ov = packet[0] & X_OVFL;
              parsing.y_ov = packet[0] & Y_OVFL;
              mouse_print_packet(&parsing);
            }
          }
          if (msg.m_notify.interrupts & timer_hook) {
            timer_int_handler();
            //printf("COUNT: %d IDLE: %d\n", time_count, idle);
            if ((time_count-idle) == sys_hz()*idle_time) {
              done = true;
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

  if (mouse_unsubscribe_int()) return 1;

  if (mouse_disable_reporting()) return 1;

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

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8042.h"
#include "mouse.h"

extern bool flag_mouse;
extern uint8_t idx;
extern struct packet parsing;
extern int counter;

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
    int ipc_status;
    message msg;
    uint8_t hook;
    int r;

    // configure mouse
    if (mouse_enable_reporting() != 0) return 1;
    if (mouse_subscribe_int(&hook) != 0) return 1;
    
    while (cnt) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & hook) { 
                        //receive the number of mouse packets specified in its argument via interrupts
                        mouse_ih();
                        if (flag_mouse) continue;
                        // parse mouse packets
                        sync_bytes(); // sincronizar byte no packet
                        if (idx == 3) { // quando temos 3 bytes no mesmo packet
                            bytes_to_packet(); // fazer o packet
                            // print them
                            mouse_print_packet(&parsing);
                            idx = 0;
                            cnt--;
                        }
                    }
                    break;
                default:
                    break;
            } 
        }
    }
    // reset mouse to minix's default config
    if (mouse_unsubscribe_int() != 0) return 1;
    if (mouse_disable_reporting() != 0) return 1;
    
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    int ipc_status;
    message msg;
    uint8_t timer_hook, mouse_hook;
    int r;
    uint16_t freq = sys_hz();
    uint8_t s = 0;
    
    if (mouse_enable_reporting() != 0) return 1;
    
    if (mouse_subscribe_int(&mouse_hook) != 0) return 1;
    if (timer_subscribe_int(&timer_hook) != 0) return 1;

    while (s < idle_time) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & timer_hook) {
                        timer_int_handler();
                        if (counter % freq == 0) {
                            s++;
                        }
                    }
                    if (msg.m_notify.interrupts & mouse_hook) { 
                        //receive the number of mouse packets specified in its argument via interrupts
                        mouse_ih();
                        if (flag_mouse) continue;
                        // parse mouse packets
                        sync_bytes(); // sincronizar byte no packet
                        if (idx == 3) { // quando temos 3 bytes no mesmo packet
                            bytes_to_packet(); // fazer o packet
                            // print them
                            mouse_print_packet(&parsing);
                            idx = 0;
                        }
                        s = 0;
                        counter = 0;
                    }
                    break;
                default:
                    break;
            } 
        }
    }
    // reset mouse to minix's default config
    if (mouse_unsubscribe_int() != 0) return 1;
    
    if (mouse_disable_reporting() != 0) return 1;
    
    return 0;
}

/*
int (mouse_test_gesture)() {
    printf("%s: under construction\n", __func__);
    return 1;
}
*/

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

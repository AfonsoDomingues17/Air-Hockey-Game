// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "i8254_i8042.h"
#include "keyboard.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_VBE_mode(mode)) return 1;
  tickdelay(micros_to_ticks(1000000 * delay));
  vg_exit();
  return 0;
}

bool extern failed;
int extern count_timer;
uint8_t extern scancode;

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  /* Allow memory mapping and Map memory */
  if (vg_init(mode) == NULL) return 1;

  /* Drawn rectangle */
  if (vg_draw_rectangle(x, y, width, height, color)) return 1;

  /* Wait for ESC */
  int re, ipc_status;
  message msg;
  uint8_t bit_no;
  uint8_t size;

  if (keyboard_subscribe(&bit_no)) return 1;
  uint8_t byte[2];
  bool is_2byte = false;
  
  while (scancode != ESC) {
    if ((re = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
      printf("driver_receive failed with: %d", re);
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
        }
        break;
      
      default:
        break;
      }
    }
  }
  if (keyboard_unsubscribe()) return 1;

  /* Exit in text mode */
  vg_exit();

  return 0;
}



int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

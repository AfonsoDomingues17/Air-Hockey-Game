// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "i8042.h"

extern uint8_t scancode;
extern bool error;

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
  /* Enter specified video mode*/
  if (graphics_set_mode(mode)) return 1;

  /* Set delay */  
  tickdelay(micros_to_ticks(1000000 * delay));

  /* Return to text mode */
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
  /* Initializes the video module in graphics mode */ 
  if (vg_init(mode) == NULL) return 1;

  /* Draws rectangle */
  if (vg_draw_rectangle(x, y, width, height, color)) return 1;

  /* Wait for ESC input */
  // Subscribe to keyboard interrupts
  int ipc_status, r;
  message msg;
  uint8_t hook;

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
          if (msg.m_notify.interrupts & hook) /* subscribed interrupt */
            kbc_ih();
          break;
        default:
          break; /* no other notifications expected: do nothing */    
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (keyboard_unsubscribe_int()) return 1;

  /* Return to text mode */
  if (vg_exit()) return 1;

  return 0;                         
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* Initializes the video module in graphics mode */ 
  if (vg_init(mode) == NULL) return 1;

  /* Draw Rectangles */
  if (vg_draw_pattern(no_rectangles, first, step)) return 1;

  /* Wait for ESC input */
  // Subscribe to keyboard interrupts
  int ipc_status, r;
  message msg;
  uint8_t hook;

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
          if (msg.m_notify.interrupts & hook) /* subscribed interrupt */
            kbc_ih();
          break;
        default:
          break; /* no other notifications expected: do nothing */    
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (keyboard_unsubscribe_int()) return 1;

  /* Return to text mode */
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* Initializes the video module in graphics mode */ 
  if (vg_init(INDEXED_MODE) == NULL) return 1;

  /* Draw XPM */
  if (vg_draw_xpm(xpm, x, y)) return 1;

  /* Wait for ESC input */
  // Subscribe to keyboard interrupts
  int ipc_status, r;
  message msg;
  uint8_t hook;

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
          if (msg.m_notify.interrupts & hook) /* subscribed interrupt */
            kbc_ih();
          break;
        default:
          break; /* no other notifications expected: do nothing */    
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (keyboard_unsubscribe_int()) return 1;

  /* Return to text mode */
  if (vg_exit()) return 1;

  return 0;
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

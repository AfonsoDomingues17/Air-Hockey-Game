#include <lcom/lcf.h>

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = TIMER0_IRQ;
unsigned int time_count = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // Checking argument validity
  if (timer < 0 || timer > 2  || freq > TIMER_FREQ || freq < 19) return 1;
  // Getting timers current status
  uint8_t command;
  if (timer_get_conf(timer, &command)) return 1;
  // Flipping init mode bits and setting timer
  command = (command & 0xF) | TIMER_LSB_MSB;
  switch (timer) {
    case 0:
      command = command | TIMER_SEL0;
      break;
    case 1:
      command = command | TIMER_SEL1;
      break;
    case 2:
      command = command | TIMER_SEL2;
      break;
    default:
      return 1;
  }
  // Calculate counter value
  uint16_t count = TIMER_FREQ/freq;
  // Send command to control register
  if (sys_outb(TIMER_CTRL, command)) return 1;
  // Send counter values to timer port
  uint8_t value;
  if (util_get_LSB(count, &value)) return 1;
  if (sys_outb(TIMER_0 + timer, value)) return 1;
  if (util_get_MSB(count, &value)) return 1;
  if (sys_outb(TIMER_0 + timer, value)) return 1;
  return 0;
}

int (timer_subscribe_interrupt)() {
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_interrupt)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  time_count++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  // Checking argument validity
  if (st == NULL || timer < 0 || timer > 2) return 1;
  // Build ReadBack Command and write into control port
  uint32_t command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, command)) return 1;
  // Read response into st
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}

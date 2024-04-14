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

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1; // ERROR
  *bit_no = BIT(hook_id); // timer_hook_id = 0 => BIT(0)
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
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

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  // Checking arguments
  if (timer < 0 || timer > 2) return 1;
  // Parsing config
  union timer_status_field_val config;
  switch (field) {
    case tsf_all:
      config.byte = st;
      break;
    case tsf_initial:
      config.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;
    case tsf_mode:
    {
      uint8_t count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      if (count_mode == (BIT(2) | BIT(1))) count_mode = 2;
      if (count_mode == (BIT(2) | BIT(1) | BIT(0))) count_mode = 3;
      config.count_mode = count_mode;
      break;
    }
    case tsf_base:
      config.bcd &= TIMER_BCD;
      break;
    default:
      return 1;
  }
  // Printing in a human readable way
  if (timer_print_config(timer, field, config)) return 1;
  return 0;
}

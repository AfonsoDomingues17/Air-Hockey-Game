#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
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
      config.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
  }
  // Printing in a human readable way
  if (timer_print_config(timer, field, config)) return 1;
  return 0;
}

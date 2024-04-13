#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook = 0;
int count_timer = 0;

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t temp;
  if (sys_inb(port, &temp)) return 1; 
  *value = (uint8_t) (temp & 0xFF);

  return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ) return 1;
  if (timer > 2 || timer < 0) return 1;

  uint8_t config;
  if (timer_get_conf(timer, &config)) return 1;
  config = (config & 0x0F) | TIMER_LSB_MSB;

  uint8_t port;
  switch (timer)
  {
  case 0:
    config = config | TIMER_SEL0;
    port = TIMER_0;
    break;
  
  case 1:
    config = config | TIMER_SEL1;
    port = TIMER_1;
    break;

  case 2:
    config = config | TIMER_SEL2;
    port = TIMER_2;
    break;

  default:
  break;
  }

  uint16_t calculo = TIMER_FREQ / freq;
  if (calculo > INT16_MAX) return 1;
  uint8_t left;
  uint8_t right;
  if (util_get_MSB(calculo, &left)) return 1;
  if (util_get_LSB(calculo, &right)) return 1;

  if (sys_outb(TIMER_CTRL, config)) return 1;
  if (sys_outb(port, right)) return 1;
  if (sys_outb(port, left)) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE ,&timer_hook);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook);
}

void (timer_int_handler)() {
  count_timer++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) return 1;
  uint8_t control_word = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, control_word)) return 1;
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer > 2 || timer < 0) return 1;

  union timer_status_field_val conf;
  switch (field) {
    case tsf_all:
      conf.byte = st;
    break;
    case tsf_initial:
      conf.in_mode = ((st & TIMER_LSB_MSB) >> 4) ;
    break;
    case tsf_mode:
    st = st & (0x0E);
    switch (st)
    {
    case 6:
      conf.count_mode = 2;
      break;
    case 7:
      conf.count_mode = 3;
      break;
    default:
      conf.count_mode = st;
      break;
    }
    case tsf_base:
      conf.bcd = (st & TIMER_BCD);
    break;
    default:
    return 1;
  }
  if (timer_print_config(timer, field, conf)) return 1;
  return 0;
}


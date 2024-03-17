#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || timer < 0) return 1;
  if (freq > TIMER_FREQ) return 1;

  uint8_t controlWord;
  if (timer_get_conf(timer, controlWord)) return 1;
  controlWord = (controlWord & 0xF) | TIMER_LSB_MSB;

  switch (timer)
  {
  case 0:
  controlWord = controlWord | TIMER_SEL0;
    break;
  case 1:
  controlWord = controlWord | TIMER_SEL1;
  case 2:
  controlWord = controlWord | TIMER_SEL2;
  default:
    break;
  }

  uint16_t calculo = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  if (util_get_LSB(calculo, &LSB)) return 1;
  if (util_get_MSB(calculo, &MSB)) return 1;

  if (sys_outb(TIMER_CTRL, controlWord)) return 1;
  if (sys_outb(TIMER_0 + timer, LSB)) return 1;
  if (sys_outb(TIMER_0 + timer, MSB)) return 1;
  }

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_ENABLE, &timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  //conta as interrupções
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) return 1;
  uint8_t RB = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, RB)) return 1;
  if (util_sys_inb(TIMER_0 + timer, st));
  return 1;
}

int (timer_display_conf) (uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer > 2 || timer < 0) return 1;
  union timer_status_field_val temp;

  switch (field)
  {
  case tsf_all:
    temp.byte = st;
    break;
  case tsf_initial: //COUNTER INITIALIZATION DO READBACK
    temp.in_mode = (st & TIMER_LSB_MSB) >> 4;
  case tsf_mode: //PROGRAMMED MODE DO READBACK
    st = (st & 0x0E) >> 1;
    switch (st)
    {
    case 6:
    temp.count_mode = 2;
    break;
    case 7:
    temp.count_mode = 3;
    break;
    default:
    temp.count_mode = st;
      break;
    }
    
  case tsf_base: //BCD DO READBACK
  temp.bcd = st & TIMER_BCD;
  default:
  return 1;
    break;
  }

  if (timer_print_config(timer, field, temp)) return 1;
  return 1;
}

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || timer < 0) return 1;
  if (freq > TIMER_FREQ) return 1;
  
  uint8_t control_word;
  if (timer_get_conf(timer, &control_word) != 0) return 1;
  control_word = (control_word & 0x0F) | TIMER_LSB_MSB;

  uint8_t port;
  switch (timer)
  {
  case 0:
    control_word = control_word | TIMER_SEL0;
    port = TIMER_0;
    break;
  case 1:
    control_word = control_word | TIMER_SEL1;
    port = TIMER_1;
    break;
  case 2:
    control_word = control_word | TIMER_SEL2;
    port = TIMER_2;
    break;  
  default:
    break;
  }
  
  uint32_t squareWaveFreq = TIMER_FREQ / freq;
  if (squareWaveFreq > INT16_MAX) return 1;
  uint8_t left;
  uint8_t right;
  util_get_LSB(squareWaveFreq, &left);
  util_get_MSB(squareWaveFreq, &right);

  if (sys_outb(TIMER_CTRL, control_word) != 0) return 1;
  if (sys_outb(port, left) != 0) return 1;
  if (sys_outb(port, right) != 0) return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  //diz ao sistema que quer começar interrupts para um diapositivo
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook_id);
  //primeiro argumento é quais diapositivos o driver vai usar
  //segundo é qual a politica o diapositivo que iniciou o interrupt vai usar
  //terceiro é um apontador identificando em qual posição do primeiro argumento o diapositivo que quer o interrupt está, neste caso é o timer
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  //conta as interrupções
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || timer < 0 || st == NULL) return 1;
  uint8_t rb = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, rb) != 0) return 1;

  if (util_sys_inb(TIMER_0 +  timer, st) != 0) return 1;

  return 0;
}

int (timer_display_conf) (uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer < 0 || timer > 2) return 0;
  union timer_status_field_val temp;

  switch (field)
  {
  case tsf_all :
    temp.byte = st;
    break;
  case tsf_initial :
    temp.in_mode = (st >> 4) & TIMER_LSB_MSB ;
  case tsf_mode :
    st = (st & 0x0E) >> 1;
    switch (st)
    {
    case 6:
      temp.count_mode = 2;
      break;
    case 7:
      temp.count_mode = 3;
    default:
      temp.count_mode = st;
      break;
    }
  case tsf_base : 
    temp.bcd = st & TIMER_BCD;
  default:
    return 1;
  }
  
  if (timer_print_config(timer, field, temp) != 0) return 1;

  return 0;
}

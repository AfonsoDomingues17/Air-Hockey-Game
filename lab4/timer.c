#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || timer < 0 || timer > 2) return 1; // verificar inputs
  
  uint8_t cw; // control word
  if (timer_get_conf(timer, &cw) != 0) return 1; // ler configuração do timer

  // read-back command -> 11 ; control word -> 11 -> LSB followed by MSB
  cw = (cw & 0xF) | TIMER_LSB_MSB; // preserva 4 bits menos significativos (pode mudar o modo de operação do timer)

  // valor inicial do contador, msb e lsb do mesmo
  uint32_t squareWaveFreq = TIMER_FREQ / freq;
  uint8_t lsb;
  util_get_LSB(squareWaveFreq, &lsb);
  uint8_t msb;
  util_get_MSB(squareWaveFreq, &msb);

  uint8_t port;
  switch (timer) { // atualiza control word de acordo com o tipo de timer
    case 0:
      cw = cw | TIMER_SEL0;
      port = TIMER_0; // 0x40
      break;
    case 1:
      cw = cw | TIMER_SEL1;
      port = TIMER_1; // 0x41
      break;
    case 2:
      cw = cw | TIMER_SEL2;
      port = TIMER_2; // 0x42
      break;
    default:
      return 1;
  }

  if (sys_outb(TIMER_CTRL, cw) != 0) return 1; // escreve control word no timer

  // escrevemos o valor inicial do timer, lsb seguido de msb, na porta respetiva
  if (sys_outb(port, lsb) != 0) return 1;
  if (sys_outb(port, msb) != 0) return 1; 

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1; // apontador tem de ser válido
  *bit_no = BIT(hook); // máscara a utilizar
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook) != 0) return 1; // subscreve interrupções
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook) != 0) return 1; // desubscreve interrupções
  return 0;
}
 
void (timer_int_handler)() { // timer 0 interrupt handler
  counter++; // increments counter
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2 || st == NULL) return 1; // verificar inputs
  // TIMER_RB_CMD = 11
  // TIMER_RB_COUNT_ = 1 (underscore final significa negado)
  // TIMER_RB_SEL(timer) = (TIMER_0 + timer) << 1
  // 1110???0
  uint8_t rb = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); // read-back command
  if (sys_outb(TIMER_CTRL, rb) != 0) return 1; // escreve read-back command no timer
  if (util_sys_inb(TIMER_0 + timer, st) != 0) return 1; // ler configuração do registo do timer
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if (timer < 0 || timer > 2) return 1; // verificar inputs
  
  union timer_status_field_val configuration; // união para armazenar os fields do status do timer

  switch (field) {
    case tsf_all: // status
      configuration.byte = st; 
      break;
    case tsf_initial:// initialization mode
      configuration.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;
    case tsf_mode: // counting mode, 0...5
      st = (st >> 1) & 0x07; // remover bit menos significativo (não tem informações sobre o modo de contagem do timer) e 0x07 para garantir que apenas os 3 bits menos significativos sejam considerados
      if (st == 6) { // 110 -> modo 2
        configuration.count_mode = 2;
      } else if (st == 7) { // 111 -> modo 3
        configuration.count_mode = 3;
      } else {
        configuration.count_mode = st;
      }
      break;
    case tsf_base: // counting base, true if BCD
      configuration.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
  }

  if (timer_print_config(timer, field, configuration) != 0) return 1; // dar print à configuration

  return 0;
}

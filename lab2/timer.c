#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
extern int counter; //use counter variable define in lab2.c
int hook_id;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer > 2 || freq > TIMER_FREQ) return 1;
  uint8_t cw;
  uint8_t msb;
  uint8_t lsb;
  if(timer_get_conf(timer,&cw) != 0) return 1;
  cw = (cw & 0xF) | TIMER_LSB_MSB;

  uint32_t nu_ticks = TIMER_FREQ / freq;
  util_get_LSB(nu_ticks,&lsb);
  util_get_MSB(nu_ticks,&msb);
  uint8_t port;
  if(timer == 0){ cw = cw | TIMER_SEL0;
  port = TIMER_0;}
   
  else if(timer == 1 ){ cw = cw | TIMER_SEL1;
  port = TIMER_1;}

  else if(timer == 2){ cw = cw | TIMER_SEL2;
  port = TIMER_2;}

  else return 1;

  if(sys_outb(TIMER_CTRL,cw) != 0) return 1;


  if(sys_outb(port,lsb) != 0) return 1;
  if(sys_outb(port,msb) != 0) return 1;


  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    hook_id = *bit_no; // transform bit_no in a 32 bit identifier
    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0)
        return 1; 
    //the return value of irqsetpolicy() will go to hook_ind
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id)) return 1; //cancels the notification subscription

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

 if(timer > 2 || timer < 0 || st == NULL) return 1;

  uint8_t rb = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if(sys_outb(TIMER_CTRL,rb) != 0) return 1;
  if(util_sys_inb(TIMER_0 + timer,st)) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                          enum timer_status_field field) {
  
  union timer_status_field_val conf;
  switch (field)
  {
  case (tsf_all):
    conf.byte = st;
    break;

  case (tsf_initial):
    conf.in_mode = (st & TIMER_LSB_MSB) >> 4;
    break;

  case (tsf_base):
    conf.bcd = (st & TIMER_BCD);
    break;

  case (tsf_mode):
    conf.count_mode = (st >> 1) & 0x7;
  default:
    return 1;
  }
  if(timer_print_config(timer,field, conf) != 0) return 1;

  
  return 0;
}

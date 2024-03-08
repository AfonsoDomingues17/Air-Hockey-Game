#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
extern int counter_timer; 
int hook_id;

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
  counter_timer++;
}





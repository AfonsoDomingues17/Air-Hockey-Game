#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/timer.h>
#include "i8254.h"

int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int)();
int (timer_set_frequency)(uint8_t timer, uint32_t freq);
void (timer_int_handler)();
int (timer_get_conf)(uint8_t timer, uint8_t *st);
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);




#endif

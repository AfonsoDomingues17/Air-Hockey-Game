#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "i8254.h"

int (timer_subscribe_interrupt)();

int (timer_unsubscribe_interrupt)();

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

void (timer_int_handler)();

int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif

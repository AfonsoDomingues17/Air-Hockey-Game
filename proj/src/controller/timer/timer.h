#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "i8254.h"

/**
 * @brief Susbscribe to timer interrupts
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (timer_subscribe_interrupt)();

/**
 * @brief Unsusbscribe to timer interrupts
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (timer_unsubscribe_interrupt)();

/**
 * @brief Sets the operating frequency of a timer
 * 
 * @param timer The timer register to configure
 * @param freq The given operating frequency of the timer
 * @return 0 if sucessful, 1 otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Timer interrupt handler
*/
void (timer_int_handler)();

/**
 * @brief Get timer controller configuration byte
 * 
 * @param timer The timer register
 * @param st A reference to the status of the timer.
 * @return 0 if sucessful, 1 otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif

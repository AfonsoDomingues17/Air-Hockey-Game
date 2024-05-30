#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>
#include <stdint.h>

#define RTC_ADDR_REG 0x70 
#define RTC_DATA_REG 0x71
#define IRQ_RTC 8
#define RTC_MASK 5

#define rtc_update 10
#define rtc_count 11

#define DM BIT(2)
#define UIP BIT(7)

#define year 9
#define month 8
#define day 7
#define hour 4  
#define minute 2
#define second 0
 
/* Day and Time */
typedef struct {
  uint8_t yyyy;
  uint8_t mm;
  uint8_t dd;
  uint8_t h;
  uint8_t m;
  uint8_t s;
} day_time;

/* Functions */

int (rtc_subscribe)();

int (rtc_unsubscribe)();

int (rtc_get_config)(uint8_t rtc, uint8_t *st);

int (verify_updating)();

int (verify_binary)();

int (rtc_convert_binary)(uint8_t bcd);

int (rtc_update_info)();

int (rtc_read_convert)(uint8_t reg, uint8_t *info);

#endif

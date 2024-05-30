#include "rtc.h"

int rtc_hook = RTC_MASK;
day_time day_time_info;

int (rtc_subscribe)() {
  return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook);
}

int (rtc_unsubscribe)() {
  return sys_irqrmpolicy(&rtc_hook);
}

int (rtc_get_config)(uint8_t rtc, uint8_t *st) {
  if (sys_outb(RTC_ADDR_REG, rtc) != 0) return 1;
  if (util_sys_inb(RTC_DATA_REG, st) != 0) return 1;
  return 0;
}

int (verify_updating)() {
  uint8_t st;
  if (rtc_get_config(rtc_update, &st) != 0) return 1;
  return (st & UIP) ? 1 : 0;
}

int (verify_binary)() {
  uint8_t st;
  if (rtc_get_config(rtc_count, &st) != 0) return 1;
  return (st & DM) ? 1 : 0;
}

int (rtc_convert_binary)(uint8_t bcd) {
  uint8_t lsb, value = 0, scale = 1;
  for (int k = 0; k < 8; k++) {
    lsb = bcd & 0x0F;
    bcd >>= 4;
    value += (lsb * scale);
    scale *= 10;
  }
  return value;
}

int (rtc_read_convert)(uint8_t reg, uint8_t *info) {
  uint8_t rtc;
  if (rtc_get_config(reg, &rtc) != 0) return 1;
  if ( ! verify_binary()) *info = rtc_convert_binary(rtc);
  return 0;
}

int (rtc_update_info)() {
  if (verify_updating() != 0) return 1;
  if (rtc_read_convert(second, &day_time_info.s) != 0) return 1;
  if (rtc_read_convert(minute, &day_time_info.m) != 0) return 1;
  if (rtc_read_convert(hour, &day_time_info.h) != 0) return 1;
  if (rtc_read_convert(day, &day_time_info.dd) != 0) return 1;
  if (rtc_read_convert(month, &day_time_info.mm) != 0) return 1;
  if (rtc_read_convert(year, &day_time_info.yyyy) != 0) return 1;
  return 0;
}


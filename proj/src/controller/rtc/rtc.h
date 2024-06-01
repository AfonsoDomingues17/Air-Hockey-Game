#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>
#include <stdint.h>

#define RTC_ADDR_REG 0x70 /** @brief Real-Time Clock adress register */
#define RTC_DATA_REG 0x71 /** @brief Real-Time Clock data register */
#define IRQ_RTC 8 /** @brief Real-Time Clock 8 IRQ line */
#define RTC_MASK 5 /** @brief Real-Time Clock interrupt mask */

#define rtc_update 10 /** @brief Date Register A */
#define rtc_count 11 /** @brief Date Register B */

#define DM BIT(2) /** @brief Data mode bit of the RTC */
#define UIP BIT(7) /** @brief Update-in-progress bit of the RTC */

#define year 9 /** @brief Date Register adress location of year */
#define month 8 /** @brief Date Register adress location of month */
#define day 7 /** @brief Date Register adress location of day */
#define hour 4 /** @brief Date Register adress location of hour */
#define minute 2 /** @brief Date Register adress location of minute */
#define second 0 /** @brief Date Register adress location of second */
 
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

/**
 * @brief Susbscribe to Real-Time Clock interrupts
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (rtc_subscribe)();

/**
 * @brief Unsusbscribe to Real-Time Clock interrupts
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (rtc_unsubscribe)();

/**
 * @brief Get Real-Time Clock configuration byte
 * 
 * @param st A reference to the status of the Real-Time Clock
 * @return 0 if sucessful, 1 otherwise
*/
int (rtc_get_config)(uint8_t rtc, uint8_t *st);

/**
 * @brief Verify if the Real-Time Clock is currently updating
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (verify_updating)();

/**
 * @brief Verify if the Real-Time Clock status is in binary mode
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (verify_binary)();

/**
 * @brief Converts a Binary-Coded Decimal (BCD) number into a binary number
 * 
 * @param bcd Binary-Coded Decimal (BCD) number to be converted
 * @return 0 if sucessful, 1 otherwise
*/
int (rtc_convert_binary)(uint8_t bcd);

/**
 * @brief Populate all fields of the day_time struct.
 * 
 * @see rtc_read_convert
 * @return 0 if sucessful, 1 otherwise
*/
int (rtc_update_info)();

/**
 * @brief Reads data from a specific register of the Real-Time Clock (RTC) and converts it into information
 * 
 * @param reg The register of the RTC from which to read data
 * @param info A pointer to where the converted information will be stored
 * @return 0 if sucessful, 1 otherwise
 */
int (rtc_read_convert)(uint8_t reg, uint8_t *info);

#endif

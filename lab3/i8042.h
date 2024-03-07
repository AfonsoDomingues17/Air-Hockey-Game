#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */

#define KEYBOARD1_IRQ 1 /**< @brief Keyboard 1 IRQ line */

/* I/O port addresses */

#define KBC_OUT_BUF   0x60 /**< @brief Keyboard data register */
#define KBC_ST_REG    0x64 /**< @brief Keyboard control register */
#define KBC_CMD_REG    0x64 /**< @brief Keyboard control register */

/* Timer control */

#define KBC_OBF BIT(0) /**< @brief Input Buffer Full status bit*/
#define KBC_IBF BIT(1) /**< @brief Input Buffer Full status bit*/
#define KBC_TO_ERR BIT(6) /**< @brief Timeout Error*/
#define KBC_PAR_ERR BIT(7) /**< @brief Parity Error*/

#define BREAK_CODE_BIT BIT(7) /**< @brief Break Code bit*/
#define TWO_BYTE_SCANCODE 0xE0 /**< @brief First byte of a two byte scancode*/

/* Important Scancodes */

#define ESC_BREAK_CODE 0x81 /**< @brief Esc key break code*/

/* Arbitrary Constants */

#define WAIT_KBC    20000 /**< @brief Time it waits before retrying to read */
#define KBC_RETRY_CNT 10 /**< @brief Number of times keyboard tries to read before giving up*/

/**@}*/

#endif /* _LCOM_I8042_H */

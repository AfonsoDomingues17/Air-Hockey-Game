#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD1_IRQ 1 /**< @brief Keyboard 1 IRQ line */
#define KEYBOARD_MASK BIT(KEYBOARD1_IRQ) /**< @brief Keyboard interrupt mask*/

/* I/O port addresses */
#define KBC_OUT_BUF   0x60 /**< @brief Keyboard data register */
#define KBC_ST_REG    0x64 /**< @brief Keyboard control register */
#define KBC_CMD_REG    0x64 /**< @brief Keyboard control register */

/* Timer control */
#define KBC_OBF BIT(0) /**< @brief Output Buffer Full status bit */
#define KBC_IBF BIT(1) /**< @brief Input Buffer Full status bit */
#define KBC_AUX BIT(5) /**< @brief Mouse status bit */
#define KBC_TO_ERR BIT(6) /**< @brief Timeout Error */
#define KBC_PAR_ERR BIT(7) /**< @brief Parity Error */

#define BREAK_CODE_BIT BIT(7) /**< @brief Break Code bit */
#define TWO_BYTE_SCANCODE 0xE0 /**< @brief First byte of a two byte scancode */

/* KBC Commands */
#define KBC_READ_CMD 0x20 /**< @brief Read Command Byte */
#define KBC_WRITE_CMD 0x60 /**< @brief Write Command Byte */

#define KBC_INT_ENBL BIT(0) /**< @brief Command bit that enables keyboard interrupts*/

/* Important Scancodes */
#define ESC_BREAK_CODE 0x81 /**< @brief Esc key break code */
#define SPACE_BREAK_CODE 0xb9 /**< @brief Space key break code */
#define S_BREAK_CODE 0x1F
#define W_BREAK_CODE 0x11

/* Arbitrary Constants */
#define WAIT_KBC    20000 /**< @brief Time it waits before retrying to read */
#define KBC_RETRY_CNT 10 /**< @brief Number of times keyboard tries to read before giving up */

/* Keyboard driver functions */

int (keyboard_get_conf)(uint8_t *st); /**< @brief Get kbc configuration byte */

int (keyboard_subscribe_int)(); /**< @brief Susbscribe to keyboard interrupts */

int (keyboard_unsubscribe_int)(); /**< @brief Unsubscribe to keyboard interrupts */

void (kbc_ih)(); /**< @brief Keyboard interrupt handler */

#endif /* _LCOM_I8042_H */

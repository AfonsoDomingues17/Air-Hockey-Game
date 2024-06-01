#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

#define MOUSE12_IRQ 12 /**< @brief Mouse 12 IRQ line */
#define MOUSE_MASK BIT(3) /**< @brief Mouse interrupt mask*/

/* I/O port addresses */

#define KBC_OUT_BUF   0x60 /**< @brief Keyboard data register */
#define KBC_ST_REG    0x64 /**< @brief Keyboard control register */
#define KBC_CMD_REG    0x64 /**< @brief Keyboard control register */

/* KBC control */

#define KBC_OBF BIT(0) /**< @brief Output Buffer Full status bit */
#define KBC_IBF BIT(1) /**< @brief Input Buffer Full status bit */
#define KBC_AUX BIT(5) /**< @brief Mouse status bit */
#define KBC_TO_ERR BIT(6) /**< @brief Timeout Error */
#define KBC_PAR_ERR BIT(7) /**< @brief Parity Error */

/* KBC Commands */

#define KBC_READ_CMD 0x20 /**< @brief Read Command Byte */
#define KBC_WRITE_CMD 0x60 /**< @brief Write Command Byte */

/* Arbitrary Constants */

#define WAIT_KBC    20000 /**< @brief Time it waits before retrying to read */
#define KBC_RETRY_CNT 10 /**< @brief Number of times keyboard tries to read before giving up */

/* Important Scancodes */

#define TWO_BYTE_SCANCODE 0xE0 /**< @brief First byte of a two byte scancode */
#define ESC_BREAK_CODE 0x81 /**< @brief Esc key break code */

/* Mouse Specific */

#define SYNC_BIT BIT(3) /**< @brief Bit that synchronises the first byte of a mouse packet */

#define LMB_BIT BIT(0) /**< @brief Bit that if set indicates Left Mouse Button is pressed */
#define RMB_BIT BIT(1) /**< @brief Bit that if set indicates Right Mouse Button is pressed */
#define MMB_BIT BIT(2) /**< @brief Bit that if set indicates Middle Mouse Button is pressed */
#define MSB_X_DELTA BIT(4) /**< @brief Most Significant Bit of the X axis displacement */
#define MSB_Y_DELTA BIT(5) /**< @brief Most Significant Bit of the Y axis displacement */
#define X_OVFL BIT(6); /**< @biref Bit that if set indicates that there has been an X axis overflow */
#define Y_OVFL BIT(7); /**< @biref Bit that if set indicates that there has been an Y axis overflow */

/* Command */

#define MOUSE_WRITE_CMD 0xD4 /**< @brief KBC command that tells it to write byte to mouse */
#define DSABL_DATA_REPORTING 0xF5 /**< @biref Mouse command that disables data reporting */
#define ENBL_DATA_REPORTING 0xF4 /**< @biref Mouse command that enables data reporting */

#define MOUSE_ACK 0xFA /**< @brief response byte from mouse that acknowledges command */

/**
 * @brief Get kbc configuration byte
 * 
 * @param st A reference to the status of the mouse.
 * @return 0 if sucessful, 1 otherwise
*/
int (mouse_get_conf)(uint8_t *st);

/**
 * @brief Susbscribe to mouse interrupts
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (mouse_subscribe_int)();

/**
 * @brief Unsusbscribe to mouse interrupts
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
*/
void (mouse_ih)();

/**
 * @brief Writes a command to the KBC_CMD_REG
 * 
 * @param cmd The command to be written to the KBC_CMD_REG
 * @return 0 if sucessful, 1 otherwise
 */
int (kbc_write_cmd)(uint8_t cmd);

/**
 * @brief Writes a command to the KBC_OUT_BUF
 * 
 * @param cmd The command to be written to the KBC_OUT_BUF
 * @return 0 if sucessful, 1 otherwise
 */
int (kbc_write_argument)(uint8_t cmd);

/**
 * @brief Reads the response from the KBC_OUT_BUF
 * 
 * @param cmd A reference to store the response received from KBC_OUT_BUF
 * @return 0 if sucessful, 1 otherwise
 */
int (kbc_read_return)(uint8_t* cmd);

/**
 * @brief Auxiliar function
 * 
 * @see kbc_write_cmd
 * @see kbc_write_argument
 * @see kbc_read_return
 * @param cmd The command to be written to the KBC_OUT_BUF.
 * @return 0 if sucessful, 1 otherwise
 */
int (write_mouse_cmd)(uint8_t cmd);

/**
 * @brief Attempts to enable the mouse by sending a command repeatedly in a loop.
 * 
 * @return 0 if sucessful, 1 otherwise
 */
int (mouse_enable_reporting)();

/**
 * @brief Attempts to disable the mouse by sending a command repeatedly in a loop.
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (mouse_disable_reporting)();

/**
 * @brief Extends a 9-bit signed number to a 16-bit signed integer.
 * 
 * @param number The 9-bit signed number to be extended.
 * @return The 16-bit signed integer resulting from the sign extension.
 */
int16_t (nine_bit_sign_extension)(uint16_t number);

#endif

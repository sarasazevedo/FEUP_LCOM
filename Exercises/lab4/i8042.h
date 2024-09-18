#ifndef _I8042_H_
#define _I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC Controller.
 */

#define DELAY_US    20000
#define LIMIT_TRIES 20

/* Interrupt Lines*/

#define KEYBOARD_IRQ_LINE 1 /**< @brief  Keyboard's IRQ line */
#define MOUSE_IRQ_LINE 12


/* KBC Port addresses */

#define KBC_STAT_REG 0x64  /**< @brief  KBC's STAT_REG */
#define KBC_CTRL_REG 0x64   /**< @brief  KBC's CTRL_REG */
#define KBC_IN_BUF 0x60       /**< @brief  KBC's IN_BUF */
#define KBC_OUT_BUF 0x60   /**< @brief  KBC's OUT_BUF  */

/* KBC Status Masks */

#define PAR_ERROR_BIT BIT(7)
#define TIME_ERROR_BIT BIT(6)
#define MOUSE_DATA_BIT BIT(5)
#define IBF_BIT BIT(1)
#define OBF_BIT BIT(0)


/* KBC Commands */

#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60
#define DISABLE_KBD_INTERFACE 0xAD
#define ENABLE_KBD_INTERFACE 0xAE
#define WRITE_BYTE_TO_MOUSE 0xD4

/* Command Byte */
#define ENABLE_KEYBOARD_INT BIT(0)

/* Scancodes */
#define BREAK_CODE BIT(7)
#define ESC_BREAK_CODE 0x81
#define FIRST_OF_TWO_BYTES 0xE0


/* Mouse Acknowledgement Byte */
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

/* Mouse Command Bytes */
#define DISABLE_DATA_REPORT 0xF5
#define ENABLE_DATA_REPORT 0xF4
#define STREAM_MODE 0xEA
#define REMOTE_MODE 0xF0

/* Mouse First Byte */
#define Y_OV BIT(7)
#define X_OV BIT(6)
#define DELTA_Y_SIGNAL BIT(5)
#define DELTA_X_SIGNAL BIT(4)
#define MIDDLE_BUTTON BIT(2)
#define RIGHT_BUTTON BIT(1)
#define LEFT_BUTTON BIT(0)






#endif /* _I8042_H */

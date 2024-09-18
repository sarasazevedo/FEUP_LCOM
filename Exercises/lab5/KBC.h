#ifndef __KBC_H
#define __KBC_H

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"


/** @defgroup kbc kbc
 * @{
 *
 * Functions for using the i8042 KBC
 */

//Reads KBC's STAT_REG to get the status byte
int (kbc_get_status)(uint8_t *stat);


//Reads OUT_BUF when full, verifies data validity and if it comes from the wanted device. Parameter from_mouse should be true when we want to get data from Mouse. It should be false when we want to get data from Keyboard or get the return of a KBC Command.
int (kbc_get_out)(uint8_t *out, bool from_mouse);


//Writes to KBC's input buffers (port 0x64 or 0x60) when not full
int (kbc_write_input)(int port, uint8_t commandOrArg);

//Sends KBC command to write Command Byte and writes one
int (kbc_configure)(uint8_t commandByte);


//Sends KBC command to read Command Byte and reads it from OUT_BUF
int (kbc_get_conf)(uint8_t *commandByte);


#endif /* __KBC_H */

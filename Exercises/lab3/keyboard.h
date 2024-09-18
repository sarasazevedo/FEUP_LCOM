#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "KBC.h"



/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the Keyboard
 */

/**
 * @brief Subscribes Keyboard interrupts and disables default IH
 *
 * @param irq_set_mask address of memory to be initialized with mask with bit number keyboard_hook_id set
 * @return Return 0 upon success and non-zero otherwise
 */
int(keyboard_subscribe_int)(uint8_t *irq_set_mask);

/**
 * @brief Unsubscribes Keyboard interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int(keyboard_unsubscribe_int)();


//Configure KBC to enable interrupts on OBF from Keyboard
int (keyboard_int_enabler)();

//Configure KBC to disable interrupts on OBF from Keyboard
int (keyboard_int_disabler)();


#endif /* __KEYBOARD_H */






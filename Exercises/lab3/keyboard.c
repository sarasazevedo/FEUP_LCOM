#include <lcom/lcf.h>

#include "keyboard.h"


int keyboard_hook_id = 1;
uint8_t read_byte = 0;
bool is_byte_valid;


int (keyboard_subscribe_int)(uint8_t *irq_set_mask) {
  if (irq_set_mask == NULL) return 1;

  *irq_set_mask = BIT(keyboard_hook_id);

  if (sys_irqsetpolicy(KEYBOARD_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) return 1;  //IRQ_EXCLUSIVE to disable default IH

  return 0;

}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&keyboard_hook_id) != 0) return 1;
  return 0;
}

//Configure KBC to enable interrupts on OBF from Keyboard
//NOT PERFECTLY TESTED (used in kbd_test_poll, which is failing tests)
int (keyboard_int_enabler)() {

  uint8_t commandByte;    

  if (kbc_get_conf(&commandByte) != 0) return 1;   //Obtain previous configuration

  commandByte = commandByte | ENABLE_KEYBOARD_INT; //Set bit

  if (kbc_configure(commandByte) != 0) return 1;  //Send new configuration

  return 0;

}

//Configure KBC to disable interrupts on OBF from Keyboard
//NOT TESTED, ended up not using in Lab3
int (keyboard_int_disabler)() {

  uint8_t commandByte;    
  if (kbc_get_conf(&commandByte) != 0) return 1;   //Obtain previous configuration

  commandByte = commandByte ^ ENABLE_KEYBOARD_INT;

  if (kbc_configure(commandByte) != 0) return 1;   //Send new configuration

  return 0;

}

//Reads byte from OUT_BUF, indicates if it is valid or not.
void (kbc_ih)() {
  if (kbc_get_out(&read_byte, false) == 0) is_byte_valid = true;
  else is_byte_valid = false;
}






#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "keyboard.h"
#include "KBC.h"
#include "timer.c"

#include <stdbool.h>
#include <stdint.h>

extern uint32_t sys_inb_counter;

extern uint8_t read_byte;
extern bool is_byte_valid;

extern int timerIntCounter;
extern uint8_t bytes[2];
extern int size;



int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/*
 Tests reading of Scancodes using Interrupts.
 -Subscribe to Keyboard Interrupts, disabling predefined IH
 -Call kbc_ih to handle the Interrupt (read and verify OUT_BUF validity)
 -Display the Scancodes
 -Unsubscribe Keyboard Interrupts
*/
int(kbd_test_scan)() {

  uint8_t kbd_irq_set;

  if (keyboard_subscribe_int(&kbd_irq_set) != 0) return 1;

  int ipc_status, r;
  message msg;
  bool make;

  bool end = false;
  

 
 while( !end ) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & kbd_irq_set) { /* subscribed interrupt */
                kbc_ih();

                if (is_byte_valid) {     //If a valid byte was read

                  if (read_byte != FIRST_OF_TWO_BYTES) {   //If it is a 1-byte scancode
                    bytes[size] = read_byte;
                    size++;
                    make = ((read_byte & BREAK_CODE) == 0);
                    kbd_print_scancode(make, size, bytes);
                    size = 0;
                    if (read_byte == ESC_BREAK_CODE) end = true;
                  }

                  else {  //If its a 2-byte scancode, wait for next Interrupt to get the rest before printing
                    bytes[size] = read_byte;
                    size++;
                  }

                }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }

  if (keyboard_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(sys_inb_counter) != 0) return 1;

  return 0;
}

int(kbd_test_poll)() {
  bool make;
  bool end = false;
  uint8_t bytes[2];
  int size = 0;

  while( !end ) {
    //kbc_get_out polls the STAT_REG and reads byte only when OBF
    if (kbc_get_out(&read_byte, false) != 0) return 1;

    if (read_byte != FIRST_OF_TWO_BYTES) {   //If it is a 1-byte scancode, print it
            bytes[size] = read_byte;
            size++;
            make = ((read_byte & BREAK_CODE) == 0);
            kbd_print_scancode(make, size, bytes);
            size = 0;
            if (read_byte == ESC_BREAK_CODE) end = true;
          }

        else {  //If its a 2-byte scancode, need to poll again to get the rest
            bytes[size] = read_byte;
            size++;
          }
    }
    if (keyboard_int_enabler() != 0) return 1;
    if (kbd_print_no_sysinb(sys_inb_counter) != 0) return 1;

    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {

  uint8_t timer_irq_set_mask;
  uint8_t keyboard_irq_set_mask;

  if (timer_subscribe_int(&timer_irq_set_mask) != 0) return 1;
  if (keyboard_subscribe_int(&keyboard_irq_set_mask) != 0) return 1;

  int ipc_status, r;
  message msg;
  uint8_t time = n;

  bool escReleased = false;
  uint8_t bytes[2];
  int size = 0;
  bool make;
 
  while( ((time != 0) && (!escReleased))  ) { 
     /* Get a request message. */
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				

                //Timer0 Interrupt
                if (msg.m_notify.interrupts & timer_irq_set_mask) { 
                   timer_int_handler();
                   if (timerIntCounter % 60 == 0) time--;
                }

                  //Keyboard Interrupt
                 if (msg.m_notify.interrupts & keyboard_irq_set_mask) { 
                    kbc_ih();

                    if (is_byte_valid) {
                      time = n;             //Resets countdown
                      timerIntCounter = 0;  //Timer0 "restarts" counting interrupts
                      if (read_byte != FIRST_OF_TWO_BYTES) {
                        bytes[size] = read_byte;
                        size++;
                        make = ((read_byte & BREAK_CODE) == 0);
                        kbd_print_scancode(make, size, bytes);
                        size = 0;
                        if (read_byte == ESC_BREAK_CODE) escReleased = true;
                      }
                      else {
                        bytes[size] = read_byte;
                        size++;
                      }
                    }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }

 if (timer_unsubscribe_int() != 0) return 1;
 if (keyboard_unsubscribe_int() != 0) return 1;
 if (kbd_print_no_sysinb(sys_inb_counter) != 0) return 1;

  return 0;
}

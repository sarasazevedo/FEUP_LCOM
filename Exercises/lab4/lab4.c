// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab4.h>
#include "mouse.h"
#include "KBC.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


extern int next_index;
extern struct packet pp;
extern bool mouse_complete;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  uint8_t mouse_irq_set_mask;

//First enable data report, then subscribe Interrupts

  if (mouse_enable_data_report() != 0) return 1; //Enable Data Reporting (while in Stream Mode)
  
  if (mouse_subscribe_int(&mouse_irq_set_mask) != 0) return 1;  //Subscribe Mouse Interrupts



  int ipc_status, r;
  message msg;

   while( cnt != 0)  { 
     /* Get a request message. */
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				

                //Mouse Interrupt
                if (msg.m_notify.interrupts & mouse_irq_set_mask) { 
                  mouse_ih();
                    if (mouse_complete) {   //If has complete packet
                      mouse_parse_packet();
                      mouse_print_packet(&pp);
                      cnt--;
                      next_index = 0;
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

//First unsubscribe inteturrpts, then disable data report

 if (mouse_unsubscribe_int() != 0) return 1;  //Unsubscribe Mouse Interrupts

 if (mouse_disable_data_report() != 0) return 1;  //Disable Mouse data reporting
 


 return 0;

}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}


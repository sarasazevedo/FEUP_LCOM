#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>


// Any header files included below this line should have been created by you

#include "videocard.h"
#include "videographics.h"
#include "keyboard.c"
#include "KBC.c"



extern uint8_t read_byte;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}




//Switches the video adapter to the graphics mode specified in its argument and then back again to the default text mode after delay seconds
int(video_test_init)(uint16_t mode, uint8_t delay) {

    //Switch video adapter to graphics mode specified in the argument
    if (vg_set_graphics_mode(mode) != 0) return 1;
 
   //Wait delay seconds 
   sleep(delay);

   //Switch video adapter back to Text Mode
   if (vg_exit() != 0) return 1;


  return 0;
}




int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  uint8_t keyboard_irq_set_mask;

  //First initialize videocard's variables with submode info
  if (set_submode_info(mode) != 0) return 1;                          
  
  //Map the video memory to the process' address space
  if (map_video_memory(mode) != 0) return 1;

  //Change the video mode to that in its argument
  if (vg_set_graphics_mode(mode) != 0) return 1;

  //Draw a rectangle
  if (vg_draw_rectangle(x, y, width, height, color) != 0) return 1; 

  //Draw a horizontal line (testing)
  //vg_draw_hline(x, y, width, color);



  if (keyboard_subscribe_int(&keyboard_irq_set_mask) != 0) return 1;

  //Reset the video mode to Minix's default text mode and return, upon receiving the break code of the ESC key (0x81)

  int ipc_status, r;
  message msg;
  bool done = false;
 
 while( !done ) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & keyboard_irq_set_mask) { /* subscribed interrupt */
                kbc_ih();
                if (read_byte == ESC_BREAK_CODE) {
                  done = true;
                  vg_exit();
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
  
  return 0;

}



int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  
  //Initialize the videocard's global variables with values relative to the given mode
  if (set_submode_info(mode) != 0) return 1;
  if (map_video_memory(mode) != 0) return 1;
 

  //Change the video mode to the specified one
  if (vg_set_graphics_mode(mode) != 0) return 1;

  //Draw a pattern of colored (filled) rectangles on the screen
  if (vg_draw_pattern(mode, no_rectangles, first, step) != 0) return 1;


  //Reset the video mode to Minix's default text mode upon receiving the break code of the ESC key

  uint8_t keyboard_irq_set_mask;

  if (keyboard_subscribe_int(&keyboard_irq_set_mask) != 0) return 1;

  int ipc_status, r;
  message msg;
  bool done = false;
 
 while( !done ) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & keyboard_irq_set_mask) { /* Keyboard Interrupt */
                    kbc_ih();
                    if (read_byte == ESC_BREAK_CODE) {
                      vg_exit();
                      done = true;
                      break;
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

  return 0;
}









int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
 
  //change to video mode 0x105
  if (set_submode_info(INDEXED_SUBMODE) != 0) return 1;
  if (map_video_memory(INDEXED_SUBMODE) != 0) return 1;
  if (vg_set_graphics_mode(INDEXED_SUBMODE) != 0) return 1;


  //display the pixmap provided as an XPM at the specified coordinates
  if (vg_draw_xpm(xpm, x, y) != 0) return 1;

  //reset the video mode to Minix's default text mode and return
  uint8_t keyboard_irq_set_mask;

  if (keyboard_subscribe_int(&keyboard_irq_set_mask) != 0) return 1;

  int ipc_status, r;
  message msg;
  bool done = false;
 
 while( !done ) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & keyboard_irq_set_mask) { /* Keyboard Interrupt */
                    kbc_ih();
                    if (read_byte == ESC_BREAK_CODE) {
                      vg_exit();
                      done = true;
                      break;
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

  return 0;
}



int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}




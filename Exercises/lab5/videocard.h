#ifndef __VIDEOCARD_H
#define __VIDEOCARD_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "VBE.h"

/* ---------------- Functions to initialize global variables -------------------------*/
int (set_submode_info)(uint16_t mode);
int (map_video_memory)(uint16_t mode);



/* ----------- Getter Functions to use global variable functions in Graphics file ------------------------ */
uint16_t (get_hres)();
uint16_t (get_vres)();
uint16_t (get_bitsPerPixel)();
//Related to color components, transforming the values to int to use with bitwise operations
int (get_redMaskSize)();
int (get_greenMaskSize)();
int (get_blueMaskSize)();
int (get_redFieldPosition)();
int (get_greenFieldPosition)();
int (get_blueFieldPosition)();



/* ----------- Helper Functions that perform calculations ------------------------ */
void (set_bytesPerPixel)();
void (set_frameBufferSize)();



/* ----------- Helper Functions to deal with pixels ------------------------ */
int (get_pixel_index)(uint16_t x, uint16_t y);
int (get_pixel_offset)(uint16_t x, uint16_t y);
int (fill_pixel)(uint16_t x, uint16_t y, uint32_t color);


#endif /* __VIDEOCARD_H */


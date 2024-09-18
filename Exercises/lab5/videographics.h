#ifndef __VIDEOGRAPHICS_H
#define __VIDEOGRAPHICS_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#include "VBE.h"
#include "videocard.h"


//Struct to store the RGB components of a color separatly, each color has a maximum size of 8 bits for the given possible submodes
typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_8_8_8_t;


//Configures the videocard to work in Graphics Mode in the chosen submode 
int (vg_set_graphics_mode)(uint16_t mode);



/* ------------- Functions to draw various different shapes ------------------*/
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);


/* -------------Helper Functions to construct and deconstruct colors into its RGB components according to the chosen submode ----------*/
void (get_color_components)(uint32_t color,  rgb_8_8_8_t *rgb);
void (get_full_color)(rgb_8_8_8_t colorRGB, uint32_t *color);
void (find_rectangle_pattern_color)(uint16_t submode, uint8_t no_rectangles, uint32_t first, uint8_t step, int row, int col, uint32_t *color );



#endif /* __VIDEOGRAPHICS_H */


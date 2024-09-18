#include <lcom/lcf.h>

#include "videographics.h"
#include <math.h>


//Configures the videocard to work in Graphics Mode in the chosen submode using a VBE Function to do that
int (vg_set_graphics_mode)(uint16_t mode) {
  reg86_t r86;    //Create a reg86_t variable
  
   //Initialize the struct's parameters with zero
  memset(&r86, 0, sizeof(r86));	

  //Define the struct's parameters with the correct values
  r86.intno = INT_VIDEO_SERVICES;  //INT 0X10 to use BIOS video services
  r86.ah = AH_VBE_FUNCTION;      //VBE Function "Set VBE Mode" 0x4f
  r86.al = AL_SET_VBE_MODE;      //VBE Function "Set VBE Mode" 0x02
  r86.bx = mode | LINEAR_MEMORY;   //Submode with bit 14 set to work in linear mode

  if( sys_int86(&r86) != 0 ) {
    return 1;
  }
  return 0;
}





/* ------------- Functions to draw various different shapes ------------------------------------------*/


//Draws a horizontal line with the specified length and color, starting at the specified coordinates (x,y)
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

    //Test if initial position is valid
    if (x < 0 | y < 0 | x > (get_hres()- 1) | y > (get_vres() -1) )
        return 1;

     //Keep the same y and change the x along the horizontal line while filling the pixels
    for (int i=0; i<len; i++) {    
            if (fill_pixel(x+i, y, color) != 0) return 1;
    }
 return 0;
}




//Draws a filled rectangle with the specified width, height and color, starting at the specified coordinates
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

    //Test if initial position is valid
    if (x < 0 | y < 0 | x > (get_hres()-1) | y > (get_vres() -1) )
        return 1;

    //For the same line keep the y, change y for each line
    for (int i=0; i<height; i++ ) {
             if (vg_draw_hline(x, y+i, width, color) != 0) return 1;

    }
    return 0;
}


int (vg_draw_pattern)(uint16_t submode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    //Find the width and height of each rectangle
    uint16_t width = floor(get_hres() / no_rectangles);
    uint16_t height = floor(get_vres() / no_rectangles);

    uint16_t x, y;
    uint32_t color;

    //Traverse the "rectangle matrix", for each row, change the col
    for (int row=0; row<no_rectangles; row++) {
      for (int col=0; col < no_rectangles; col++) {
        //For each rectangle obtain its initial screen coordinates (x,y) and color to then call vg_draw_rectangle
        x = col * width;
        y = row * height;
        find_rectangle_pattern_color(submode, no_rectangles, first, step, row, col, &color);
        if (vg_draw_rectangle(x,y, width, height, color) != 0) return 1;

      }

    }
    return 0;

 }

 int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
     xpm_image_t img;

      uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &img);
      if (sprite == NULL) return 1;

      for (int h=0; h<img.height; h++) {
        for (int w=0; w<img.width; w++) {
          fill_pixel(x+w, y+h, *sprite);
          sprite++;
        }
      }
      return 0;
 }


/* -------------Helper Functions to construct and deconstruct colors into its RGB components according to the chosen submode ----------*/

//From a 32bit color, find its RGB components and store them separatly in a struct
void (get_color_components)(uint32_t color,  rgb_8_8_8_t *rgb) {
   //From the 32bit color obtain its Red component using the current submode's  redFieldPosition, redMaskSize and bitwise operations
    rgb->red= (((BIT(get_redMaskSize()) - 1) << get_redFieldPosition()) & color) >> get_redFieldPosition();
    //Green
    rgb->green = (((BIT(get_greenMaskSize()) - 1) << get_greenFieldPosition()) & color) >> get_greenFieldPosition();
    //Blue
    rgb->blue = (((BIT(get_blueMaskSize()) - 1) << get_blueFieldPosition()) & color) >> get_blueFieldPosition();
 }


//According to the current submode, join the color's RGB components building a 32bit color, if the submode uses less than 32 bits per color, the MSB are 0 besides the needed value
void (get_full_color)(rgb_8_8_8_t colorRGB, uint32_t *color) {

    *color = (0x0 | (colorRGB.red << get_redFieldPosition()) | (colorRGB.green << get_greenFieldPosition()) | (colorRGB.blue << get_blueFieldPosition()));
}


//Helper function to transform colors as asked in Lab5's "video_test_pattern" function
void (find_rectangle_pattern_color)(uint16_t submode, uint8_t no_rectangles, uint32_t first, uint8_t step, int row, int col, uint32_t *color ) {
      rgb_8_8_8_t firstRGB, currentRGB;
      
      //If in Indexed mode
      if (submode == INDEXED_SUBMODE) {
        *color = (first + (row * no_rectangles + col) * step) % (1 << (int) get_bitsPerPixel());
      }
      //If in Direct Mode
      else {

        //Separate the "first" color in its RGB Components
        get_color_components(first, &firstRGB); 

        //Calculate the current rectangle's color RGB Components as asked in Lab5 handout
        
        //Red (R(row, col) = (R(first) + col * step) % (1 << RedMaskSize))
        currentRGB.red= (firstRGB.red + col * step) % (1 << (int) get_redMaskSize());

        //GreenG (row, col) = (G(first) + row * step) % (1 << GreenMaskSize)
        currentRGB.green = (firstRGB.green + row * step) % (1 << (int) get_greenMaskSize());

        //Blue B(row, col) = (B(first) + (col + row) * step) % (1 << BlueMaskSize)
        currentRGB.blue= (firstRGB.blue + (col + row) * step) % (1 << (int) get_blueMaskSize);

        //Join the current color RGB components in a 32 bit color according to the current submode
        get_full_color(currentRGB, color);

      }

 }




#include <lcom/lcf.h>

#include <string.h>

#include "videocard.h"


/* Variables to store information about the videocard's current submode and associated frame-buffer*/

static uint16_t submode;  /* Graphics Submode */
static uint16_t hres;  /* XResolution */
static uint16_t vres;  /* YResolution */
static uint8_t bitsPerPixel;  /* BitsPerPixel*/
static uint32_t physicalAddress;  /* PhysBasePtr */

static uint8_t redMaskSize;
static uint8_t greenMaskSize;
static uint8_t blueMaskSize;
static uint8_t redFieldPosition;
static uint8_t greenFieldPosition;
static uint8_t blueFieldPosition;

static int bytesPerPixel;  /* No of bytes per pixel in the frame buffer*/
static int frameBufferSize;  /* Number of bytes in the frame_buffer */

static uint8_t *frame_buffer;		/* Virtual address to which VRAM is mapped */



/* ---------------- Functions to initialize global variables -------------------------*/


//Initializes the videocard's global variables with information about the chosen submode
int (set_submode_info)(uint16_t mode) {
  vbe_mode_info_t mode_info;
  memset(&mode_info, 0, sizeof(mode_info));  //Initialize parameters with zero
  
  if (vbe_get_mode_info(mode, &mode_info) != 0) return 1;
  
  //Parameters from mode_info struct
  submode = mode;
  hres = mode_info.XResolution;
  vres = mode_info.YResolution;
  bitsPerPixel = mode_info.BitsPerPixel;
  physicalAddress = mode_info.PhysBasePtr;
  redMaskSize = mode_info.RedMaskSize;
  greenMaskSize = mode_info.GreenMaskSize;
  blueMaskSize = mode_info.BlueMaskSize;
  redFieldPosition = mode_info.RedFieldPosition;
  greenFieldPosition = mode_info.GreenFieldPosition;
  blueFieldPosition = mode_info.BlueFieldPosition;


  //Calculate bytesPerPixel and frameBufferSize using the previous found values
  set_bytesPerPixel();
  set_frameBufferSize();


  return 0;
}


//Initializes the videocard's frame_buffer global variable with the initial virtual address of a frame-buffer with the correct size for the chosen submode
int (map_video_memory)(uint16_t mode) {

  int r;
  struct minix_mem_range mr; /* physical memory range */
  memset(&mr, 0, sizeof(mr));	
  unsigned int vram_size = frameBufferSize;
  unsigned int vram_base = physicalAddress;

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) return 1;

  /* Map memory */
  frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(frame_buffer == MAP_FAILED) return 1;

  return 0;

}


/* ----------- Getter Functions to use global variable functions in Graphics file ------------------------ */

uint16_t (get_hres)() {
  return hres;
}


uint16_t (get_vres)() {
  return vres;
}

uint16_t (get_bitsPerPixel)() {
  return bitsPerPixel;
}

//Related to color components, transforming the values to int to use with bitwise operations
int (get_redMaskSize)() {
  return (int)redMaskSize;
}

int (get_greenMaskSize)() {
  return (int)greenMaskSize;
}

int (get_blueMaskSize)() {
  return (int) blueMaskSize;
}

int (get_redFieldPosition)() {
  return (int)redFieldPosition;
}

int (get_greenFieldPosition)() {
  return (int)greenFieldPosition;
}

int (get_blueFieldPosition)() {
  return (int)blueFieldPosition;
}




/* ----------- Helper Functions that perform calculations ------------------------ */

//Find how many bytes per pixel are used for the current videocard's submode, rounding up
void (set_bytesPerPixel)() {
  if ((bitsPerPixel % 8) == 0) {
      bytesPerPixel = (bitsPerPixel / 8);
  }
  else {
    bytesPerPixel= ((bitsPerPixel + 7) / 8);
  }

}


//Find how many bytes the videocard's frame-buffer has
void (set_frameBufferSize)() {

  int noPixels = vres * hres;

  frameBufferSize = (noPixels * bytesPerPixel);

}



/* ----------- Helper Functions to deal with pixels ------------------------ */


//Return the index of pixel (x,y) in the sequential list of pixels (in linear mode, starting with index 0)
int (get_pixel_index)(uint16_t x, uint16_t y) {
  return (y * hres + x);

}

//Find the offset of pixel (x,y) in the frame-buffer memory
int (get_pixel_offset)(uint16_t x, uint16_t y) {
  int pixel_index = get_pixel_index(x,y);

  return (pixel_index * bytesPerPixel);

}
//Paints pixel (x,y) with the specified color by copying the color's bytes to the pixel's memory area
int (fill_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    //Verify if pixel coordinates are valid
    if (x<0 | y < 0 | x> (hres-1) | y>(vres-1)) return 1;

    int pixel_offset = get_pixel_offset(x,y);

    //memcpy(void *dest, const void *src, size_t n);
    if (memcpy(&frame_buffer[pixel_offset], &color, bytesPerPixel) == NULL) return 1;

    return 0;

}













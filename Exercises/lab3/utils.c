#include <lcom/lcf.h>

#include <stdint.h>

#ifdef LAB3
uint32_t sys_inb_counter = 0;
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = val & 0xFF;
  return 0;
 

}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (val >> 8) & 0xFF;
  return 0;
}


int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t val;
  if (sys_inb(port, &val) != 0) return 1;
  *value = 0xFF & val;

  #ifdef LAB3
  sys_inb_counter++;
  #endif

  return 0;
}

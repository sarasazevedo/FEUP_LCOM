#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/** @defgroup VBE VBE
 * @{
 *
 * Constants for programming the Videocard.
 */


#define INT_VIDEO_SERVICES 0x10

#define AH_VBE_FUNCTION 0x4f
#define AH_BIOS_FUNCTION 0x00
#define AL_SET_VBE_MODE 0x02

#define INDEXED_SUBMODE 0x105
#define SUBMODE_110 0x110
#define SUBMODE_115 0x115
#define SUBMODE_11A 0x11A
#define SUBMODE_14C 0x14C

#define LINEAR_MEMORY BIT(14)


/**@}*/

#endif /* _LCOM_VBE_H */

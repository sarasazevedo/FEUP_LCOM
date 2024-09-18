#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"



void (mouse_ih)();

void (mouse_add_byte)();

void (mouse_parse_packet)();

int (mouse_subscribe_int)(uint8_t *irq_set_mask);


int (mouse_unsubscribe_int)();


int (mouse_write_byte)(uint8_t byte);

int (mouse_disable_data_report)();


int (mouse_enable_data_report)();


int (mouse_set_stream_mode)();

int (mouse_set_remote_mode)();


#endif /* __MOUSE_H */

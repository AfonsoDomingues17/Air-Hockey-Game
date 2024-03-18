#ifndef MOUSE_H_
#define MOUSE_H_
#include <lcom/lcf.h>
#include <stdint.h>

int (subscribe_MOUSE_interrupts)(uint8_t *bit_no);

int (unsubscribe_MOUSE_interrupts)();

void(packet_creation)();

void(packets_assembly)();

void (mouse_ih)();

int (disable_enable_data_reporting)(uint8_t cmd);

int (enable_data)(uint8_t cmd);

#endif /* MOUSE_H_ */


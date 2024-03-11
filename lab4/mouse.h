#ifndef MOUSE_H_
#define MOUSE_H_
#include <lcom/lcf.h>
#include <stdint.h>

int (subscribe_MOUSE_interrupts)(uint8_t *bit_no);

int (unsubscribe_MOUSE_interrupts)();

int(packet_assembly)(uint8_t packet[]);

void (mouse_ih)();

#endif /* MOUSE_H_ */


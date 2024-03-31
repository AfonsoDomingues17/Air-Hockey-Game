#ifndef MOUSE_H_
#define MOUSE_H_

void read_packets();
int unsubscribe_mouse();
int subscribe_mouse(uint8_t *bit_no);
int (mouse_disable_data_reporting)();
int (mouse_enable_data_reporting)();

#endif

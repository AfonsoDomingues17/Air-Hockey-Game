#ifndef MOUSE_H_
#define MOUSE_H_

void (mouse_ih)();
int unsubscribe_mouse();
int subscribe_mouse(uint8_t *bit_no);
int (mouse_disable_data_reporting2)();
int (mouse_enable_data_reporting2)();

int write_command(uint8_t comando);
int write_arguments(uint8_t comando);
int receive_acknowledgment_byte(uint8_t *byte);
int (write_mouse_cmd)(uint8_t cmd);

#endif

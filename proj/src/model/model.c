#include "model.h"

/* Variáveis externas necessárias */
extern unsigned int time_count;
extern uint8_t scancode;
extern bool keyboard_error;
extern uint8_t byte;
extern bool mouse_error;

// Packet variables
uint8_t packet[3];
int count = 0;

/* Iniciar estado do programa */
MainStateMachine mainState = RUNNING;

/* Timers */
unsigned int idle = 0;

void (timer_int)() {
    timer_int_handler();
    draw_frame();
    if (time_count % sys_hz() == 0) idle++;
    if (idle == 100) mainState = STOP;
}

void (keyboard_int)() {
    kbc_ih();
    if (keyboard_error) return;
    if (scancode == ESC_BREAK_CODE) mainState = STOP;
    idle = time_count;
}

void (mouse_int)() {
    mouse_ih();
    if (mouse_error) return;
    if (count == 0 && (byte & SYNC_BIT) == 0) return;
    packet[count++] = byte;
    if (count == 3) {
        count = 0;
        // Parse Mouse Packet Info
        struct packet parsing;
        parse_mouse_data(&parsing);

        // Update mouse location
        mouse_update(mouse, parsing);
    }
}

void (parse_mouse_data)(struct packet* parsing) {
    for (int i = 0; i < 3; i++) {
        parsing->bytes[i] = packet[i];
    }
    parsing->rb = packet[0] & RMB_BIT;
    parsing->mb = packet[0] & MMB_BIT;
    parsing->lb = packet[0] & LMB_BIT;
    parsing->delta_x = packet[1];
    if (packet[0] & MSB_X_DELTA) {
        uint16_t temp = BIT(8) + packet[1];
        parsing->delta_x = nine_bit_sign_extension(temp);
    }
    parsing->delta_y = packet[2];
    if (packet[0] & MSB_Y_DELTA) {
        uint16_t temp = BIT(8) + packet[2];
        parsing->delta_y = nine_bit_sign_extension(temp);
    }
    parsing->x_ov = packet[0] & X_OVFL;
    parsing->y_ov = packet[0] & Y_OVFL;
}

void (mouse_update)(Sprite* mouse, struct packet parsing) {           
    // If there was a data overflow, dont update
    if (parsing.x_ov || parsing.y_ov) return;

    int new_x = mouse->x + parsing.delta_x;
    int new_y = mouse->y + parsing.delta_y;
    
    unsigned h_res = get_h_res();
    unsigned v_res = get_v_res();

    if (new_x > (int)h_res || new_x < 0) return;
    if (new_y > (int)v_res || new_y < 0) return; 

    mouse->x = new_x;
    mouse->y = new_y;
}

void (loader_sprite)() {
    mouse = create_sprite((xpm_map_t) xpm_mouse, 576, 432);
}

void unloader_sprite() {
    delete_sprite(mouse);
}

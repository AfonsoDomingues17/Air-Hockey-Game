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
MainStateMachine mainState = MAIN_MENU;
int option = 0;

/* Timers */
unsigned int idle_game = 0;

void (timer_int)() {
    timer_int_handler();
    swap_buffers();
    switch (mainState) {
        case GAME:
            if (time_count % sys_hz() == 0) idle_game++;
            break;
        default:
            break;
    }
}

void (keyboard_int)() {
    kbc_ih();
    if (keyboard_error) return;
    switch (mainState) {
        case MAIN_MENU:
            if (scancode == ESC_BREAK_CODE) mainState = STOP;
            if (scancode == SPACE_BREAK_CODE) mainState = GAME;
            //if (scancode == S_BREAK_CODE) option_down();
            if (scancode == S_BREAK_CODE) option_up();
            break;
        case GAME:
            if (scancode == ESC_BREAK_CODE) mainState = MAIN_MENU;
            idle_game = time_count;        
            break;
        default:
            break;
    }
    draw_frame();
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
    
        // Draw new frame
        draw_frame();
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
    int new_y = mouse->y - parsing.delta_y;
    
    unsigned h_res = get_h_res();
    unsigned v_res = get_v_res();

    if (new_x > (int)h_res || new_x < 0) return;
    if (new_y > (int)v_res || new_y < 0) return; 

    mouse->x = new_x;
    mouse->y = new_y;
}

void (loader_sprite)() {
    mouse = create_sprite((xpm_map_t) xpm_mouse, 576, 150);
    menu_button = create_sprite((xpm_map_t) play_unselected, 421, 350);
    menu_button->selected = true;
    menu_2button = create_sprite((xpm_map_t) play_selected, 421, 350);
    menu_3button = create_sprite((xpm_map_t) leaderboard_unselected, 421, 500);
    menu_4button = create_sprite((xpm_map_t) leaderboard_selected, 421, 500);
    menu_5button = create_sprite((xpm_map_t) exit_unselected, 421, 650);
    menu_6button = create_sprite((xpm_map_t) exit_selected, 421, 650);



}

void (unloader_sprite)() {
    delete_sprite(mouse);
    delete_sprite(menu_button);
    delete_sprite(menu_2button);
    delete_sprite(menu_3button);
    delete_sprite(menu_4button);
    delete_sprite(menu_5button);
    delete_sprite(menu_6button);
}

void (option_up)() {
    switch (option) {
        case 0:
            menu_button->selected = false;
            menu_2button->selected = false;
            menu_5button->selected = true;
            menu_6button->selected = true;
            option = 2;
            break;
        case 1:
            menu_3button->selected = false;
            menu_4button->selected = false;
            menu_button->selected = true;
            menu_2button->selected = true;
            option = 0;
            break;
        case 2:
            menu_5button->selected = false;
            menu_6button->selected = false;
            menu_3button->selected = true;
            menu_4button->selected = true;
            option = 1;
            break;
        default:
            break;
    }
}
/*
void (option_down)() {
    switch (option) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
}
*/

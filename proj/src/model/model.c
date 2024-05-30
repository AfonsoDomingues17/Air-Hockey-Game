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

extern Queue *inQueue;


/* Timers */
unsigned int idle_game = 0;

void (timer_int)() {
    timer_int_handler();
    swap_buffers();
    switch (mainState) {
        case GAME:
            if (time_count % sys_hz() == 0) idle_game++;
            if(idle_game == 120) mainState = STOP;
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
            if (exit_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = STOP;
            if (start_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = GAME;
            if (scancode == S_BREAK_CODE) option_down();
            if (scancode == W_BREAK_CODE) option_up();
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
        if(parsing.lb){
            move_puck_with_mouse(parsing);
            sendNewPositions();
            mouse->visibility = false;
        }
        else mouse->visibility = true;

    
        // Draw new frame
        draw_frame();

    }
}

void (sp_int)() {
    switch (mainState)
    {
    case GAME:
        sp_ih();
        redpuck->x = queue_read_int(inQueue);
        redpuck->y = queue_read_int(inQueue);
        draw_frame(); 
        break;
    default:
        serialPort_resetFIFO();
        break;
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

    if(new_x > 275 && new_x < 869) mouse->x = new_x;
    mouse->y = new_y;
  
}

void (move_puck_with_mouse)(struct packet parsing) {

    unsigned int new_x = bluepuck->x + parsing.delta_x;
    unsigned int new_y = bluepuck->y - parsing.delta_y;

    unsigned h_res = get_h_res();
    unsigned v_res = get_v_res();

    if (new_x + bluepuck->width < h_res) {
        if(new_x > 270 && new_x < 801) bluepuck->x = new_x;
    }

    if (new_y + bluepuck->height < v_res) {
        if(new_y > 436 && new_y < 765)bluepuck->y = new_y;
    }
}

void (loader_sprite)() {
    mouse = create_sprite((xpm_map_t) xpm_mouse, 576, 150);
    start_button_unselected = create_sprite((xpm_map_t) play_unselected, 421, 350);
    start_button_unselected->selected = true;
    start_button_selected = create_sprite((xpm_map_t) play_selected, 421, 350);
    start_button_selected->selected = true;
    leaderboard_button_unselected = create_sprite((xpm_map_t) leaderboard_unselected, 421, 500);
    leaderboard_button_selected = create_sprite((xpm_map_t) leaderboard_selected, 421, 500);
    exit_button_unselected = create_sprite((xpm_map_t) exit_unselected, 421, 650);
    exit_button_selected = create_sprite((xpm_map_t) exit_selected, 421, 650);
    redpuck = create_sprite((xpm_map_t) red_puck, 535, 55);
    bluepuck = create_sprite((xpm_map_t) blue_puck, 535, 730);
    Ball = create_sprite((xpm_map_t) disk, 500, 500);

}

void (unloader_sprite)() {
    delete_sprite(mouse);
    delete_sprite(start_button_unselected);
    delete_sprite(start_button_selected);
    delete_sprite(leaderboard_button_unselected);
    delete_sprite(leaderboard_button_selected);
    delete_sprite(exit_button_unselected);
    delete_sprite(exit_button_selected);
    delete_sprite(redpuck);
    delete_sprite(bluepuck);
    delete_sprite(Ball);
}

void (option_up)() {
    switch (option) {
        case 0:
            start_button_unselected->selected = false;
            start_button_selected->selected = false;
            exit_button_unselected->selected = true;
            exit_button_selected->selected = true;
            option = 2;
            break;
        case 1:
            leaderboard_button_unselected->selected = false;
            leaderboard_button_selected->selected = false;
            start_button_unselected->selected = true;
            start_button_selected->selected = true;
            option = 0;
            break;
        case 2:
            exit_button_unselected->selected = false;
            exit_button_selected->selected = false;
            leaderboard_button_unselected->selected = true;
            leaderboard_button_selected->selected = true;
            option = 1;
            break;
        default:
            break;
    }
}

void (option_down)() {
    switch (option) {
        case 0:
            start_button_unselected->selected = false;
            start_button_selected->selected = false;
            leaderboard_button_unselected->selected = true;
            leaderboard_button_selected->selected = true;
            option = 1;
            break;
        case 1:
            leaderboard_button_unselected->selected = false;
            leaderboard_button_selected->selected = false;
            exit_button_unselected->selected = true;
            exit_button_selected->selected = true;
            option = 2;
            break;
        case 2:
            exit_button_unselected->selected = false;
            exit_button_selected->selected = false;
            start_button_unselected->selected = true;
            start_button_selected->selected = true;
            option = 0;
            break;
        default:
            break;
    }
}


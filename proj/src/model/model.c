#include "model.h"

/* Variáveis externas necessárias */
extern unsigned int time_count;
extern uint8_t scancode;
extern bool keyboard_error;
extern uint8_t byte;
extern bool mouse_error;
extern day_time day_time_info;

// Packet variables
uint8_t packet[3];
int count = 0;

/* Iniciar estado do programa */
MainStateMachine mainState = MAIN_MENU;
int option = 0;

/* Timers */
unsigned int idle_game = 0;
unsigned int time_remaining;
unsigned int start_time;

/* Player Points */
int player_1 = 0, player_2 = 0;

/* Menu buttons */

void (timer_int)() {
    timer_int_handler();
    swap_buffers();
    switch (mainState) {
        case MAIN_MENU:
            if (time_count % sys_hz() == 0) {
                draw_frame();
            }
        case GAME:
            if (time_count % sys_hz() == 0) {
                if (time_remaining > 0) {
                    time_remaining--; 
                    draw_frame();
                } 
            }
            move(Ball, Ball->xspeed, Ball->yspeed, 1);
            draw_frame();
            break;
        default:
            break;
    }
}

void reset_option() {
    option = 0;
}

void (keyboard_int)() {
    kbc_ih();
    if (keyboard_error) return;
    switch (mainState) {
        case MAIN_MENU:
            if (exit_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = STOP;
            if (start_button_selected->selected && scancode == SPACE_BREAK_CODE) {
                mainState = GAME;
                time_remaining = 5;
                start_time = time_count / sys_hz(); 
                time_count = 0;
            }
            if (scancode == S_BREAK_CODE) option_down(buttons_menu_unselected, buttons_menu_selected, 3);
            if (scancode == W_BREAK_CODE) option_up(buttons_menu_unselected, buttons_menu_selected, 3);
            break;
        case GAME:
            if (scancode == ESC_BREAK_CODE) mainState = MAIN_MENU;
            idle_game = time_count;        
            break;
        case WIN:
            if (leave_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = STOP;
            if (play_again_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = GAME;
            if (scancode == W_BREAK_CODE) option_up(buttons_winlose_unselected, buttons_winlose_selected, 2);
            if (scancode == S_BREAK_CODE) option_down(buttons_winlose_unselected, buttons_winlose_selected, 2);
            break;
        case LOST:
            if (leave_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = STOP;
            if (play_again_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = GAME;
            if (scancode == W_BREAK_CODE) option_up(buttons_winlose_unselected, buttons_winlose_selected, 2);
            if (scancode == S_BREAK_CODE) option_down(buttons_winlose_unselected, buttons_winlose_selected, 2);
            break;
        case TIE:
            if (leave_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = STOP;
            if (play_again_button_selected->selected && scancode == SPACE_BREAK_CODE) mainState = GAME;
            if (scancode == W_BREAK_CODE) option_up(buttons_winlose_unselected, buttons_winlose_selected, 2);
            if (scancode == S_BREAK_CODE) option_down(buttons_winlose_unselected, buttons_winlose_selected, 2);
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
            move(bluepuck, parsing.delta_x, parsing.delta_y, 2);
            mouse->visibility = false;
        }
        else mouse->visibility = true;
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

    move(mouse, parsing.delta_x, parsing.delta_y, 0);
}

void (rtc_int)() {
    rtc_update_info();
}

void (loader_sprite)() {
    mouse = create_sprite((xpm_map_t) xpm_mouse, 576, 150);
    start_button_unselected = create_sprite((xpm_map_t) play_unselected, 421, 350);
    start_button_unselected->selected = true;
    start_button_selected = create_sprite((xpm_map_t) play_selected, 421, 350);
    start_button_selected->selected = true;
    redpuck = create_sprite((xpm_map_t) red_puck, 535, 55);
    bluepuck = create_sprite((xpm_map_t) blue_puck, 535, 730);
    Ball = create_sprite((xpm_map_t) disk, 500, 500);

    numbers[0] = create_sprite((xpm_map_t) white_zero, 0, 0);
    numbers[1] = create_sprite((xpm_map_t) white_one, 0, 0);
    numbers[2] = create_sprite((xpm_map_t) white_two, 0, 0);
    numbers[3] = create_sprite((xpm_map_t) white_three, 0, 0);
    numbers[4] = create_sprite((xpm_map_t) white_four, 0, 0);
    numbers[5] = create_sprite((xpm_map_t) white_five, 0, 0);
    numbers[6] = create_sprite((xpm_map_t) white_six, 0, 0);
    numbers[7] = create_sprite((xpm_map_t) white_seven, 0, 0);
    numbers[8] = create_sprite((xpm_map_t) white_eight, 0, 0);
    numbers[9] = create_sprite((xpm_map_t) white_nine, 0, 0);
    time_sep = create_sprite((xpm_map_t) white_time, 115, 425);
    time_title = create_sprite((xpm_map_t) title_time, 55, 370);

    points_title = create_sprite((xpm_map_t) title_points, 925, 370);
    points_sep = create_sprite((xpm_map_t) white_points, 1015, 440);

    two_points = create_sprite((xpm_map_t) time_two_points, 20+35*2, 15);
    two_points2 = create_sprite((xpm_map_t) time_two_points, 20+35*5, 15);

    day_sep = create_sprite((xpm_map_t) day_separator, 15+35*12, 20);
    day_sep2 = create_sprite((xpm_map_t) day_separator, 15+35*15, 20);

    numbers_blue[0] = create_sprite((xpm_map_t) blue_zero, 0, 425);
    numbers_blue[1] = create_sprite((xpm_map_t) blue_one, 0, 425);
    numbers_blue[2] = create_sprite((xpm_map_t) blue_two, 0, 425);
    numbers_blue[3] = create_sprite((xpm_map_t) blue_three, 0, 425);
    numbers_blue[4] = create_sprite((xpm_map_t) blue_four, 0, 425);
    numbers_blue[5] = create_sprite((xpm_map_t) blue_five, 0, 425);
    numbers_blue[6] = create_sprite((xpm_map_t) blue_six, 0, 425);
    numbers_blue[7] = create_sprite((xpm_map_t) blue_seven, 0, 425);
    numbers_blue[8] = create_sprite((xpm_map_t) blue_eight, 0, 425);
    numbers_blue[9] = create_sprite((xpm_map_t) blue_nine, 0, 425);

    numbers_red[0] = create_sprite((xpm_map_t) red_zero, 0, 425);
    numbers_red[1] = create_sprite((xpm_map_t) red_one, 0, 425);
    numbers_red[2] = create_sprite((xpm_map_t) red_two, 0, 425);
    numbers_red[3] = create_sprite((xpm_map_t) red_three, 0, 425);
    numbers_red[4] = create_sprite((xpm_map_t) red_four, 0, 425);
    numbers_red[5] = create_sprite((xpm_map_t) red_five, 0, 425);
    numbers_red[6] = create_sprite((xpm_map_t) red_six, 0, 425);
    numbers_red[7] = create_sprite((xpm_map_t) red_seven, 0, 425);
    numbers_red[8] = create_sprite((xpm_map_t) red_eight, 0, 425);
    numbers_red[9] = create_sprite((xpm_map_t) red_nine, 0, 425);

    play_again_button_selected = create_sprite((xpm_map_t) play_again_selected, 450, 380);
    play_again_button_selected->selected = true;
    play_again_button_unselected = create_sprite((xpm_map_t) play_again_unselected, 450, 380);
    play_again_button_unselected->selected = true;

    exit_button_unselected = create_sprite((xpm_map_t) exit_unselected, 421, 650);
    leaderboard_button_unselected = create_sprite((xpm_map_t) leaderboard_unselected, 421, 500);
    leaderboard_button_selected = create_sprite((xpm_map_t) leaderboard_selected, 421, 500);
    exit_button_selected = create_sprite((xpm_map_t) exit_selected, 421, 650);

    buttons_menu_unselected[0] = start_button_unselected;
    buttons_menu_unselected[1] = leaderboard_button_unselected;
    buttons_menu_unselected[2] = exit_button_unselected;

    buttons_menu_selected[0] = start_button_selected;
    buttons_menu_selected[1] = leaderboard_button_selected;
    buttons_menu_selected[2] = exit_button_selected;

    leave_button_selected = create_sprite((xpm_map_t) leave_selected, 495, 480);
    leave_button_unselected = create_sprite((xpm_map_t) leave_unselected, 495, 480);

    buttons_winlose_selected[0] = play_again_button_selected;
    buttons_winlose_selected[1] = leave_button_selected;

    buttons_winlose_unselected[0] = play_again_button_unselected;
    buttons_winlose_unselected[1] = leave_button_unselected;
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
    for (int i = 0; i < 3; i++) {
        delete_sprite(buttons_menu_selected[i]);
    }
    for (int i = 0; i < 3; i++) {
        delete_sprite(buttons_menu_unselected[i]);
    }
    for (int i = 0; i < 2; i++) {
        delete_sprite(buttons_winlose_selected[i]);
    }
    for (int i = 0; i < 2; i++) {
        delete_sprite(buttons_winlose_unselected[i]);
    }
    for (int i = 0; i < 10; i++) {
        delete_sprite(numbers[i]);
    }
    delete_sprite(time_sep);
    delete_sprite(time_title);
    delete_sprite(points_title);
    delete_sprite(points_sep);
    delete_sprite(two_points);
    delete_sprite(two_points2);
    delete_sprite(day_sep);
    delete_sprite(day_sep2);
    for (int i = 0; i < 10; i++) {
        delete_sprite(numbers_blue[i]);
    }
    for (int i = 0; i < 10; i++) {
        delete_sprite(numbers_red[i]);
    }
}

void (option_up)(Sprite* buttons_unselected[], Sprite* buttons_selected[], int size) {
    buttons_unselected[option]->selected = false;
    buttons_selected[option]->selected = false;
    option--;
    if (option == -1) option = size - 1;
    buttons_unselected[option]->selected = true;
    buttons_selected[option]->selected = true;
}

void (option_down)(Sprite* buttons_unselected[], Sprite* buttons_selected[], int size) {
    buttons_unselected[option]->selected = false;
    buttons_selected[option]->selected = false;
    option++;
    if (option == size) option = 0;
    buttons_unselected[option]->selected = true;
    buttons_selected[option]->selected = true;
}


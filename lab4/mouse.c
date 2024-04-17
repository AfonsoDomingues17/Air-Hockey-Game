#include <lcom/lcf.h>

#include "mouse.h"
#include "i8042.h"

int mouse_hook_id = 3;
bool flag_mouse = false;
uint8_t byte = 0;
uint8_t actual_byte;
uint8_t idx = 0; /* 0, 1, 2*/
uint8_t mouse_array[3];  
struct packet parsing;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_get_status)(uint8_t *status) {
    if (status == NULL) return 1;
    if (util_sys_inb(KBC_ST_REG, status)) return 1;
    return 0;
}

// em cada chamada, lê novo byte do rato
void (mouse_ih)() {
    uint8_t status;
    uint8_t data;
    flag_mouse = false;
    int attempts = KBC_ATTEMPTS;
    while (attempts--) {
        if (mouse_get_status(&status)) continue;
        if (status & KBC_OBF) {
            if (util_sys_inb(OUT_BUF, &data)) continue;
            if (((status & (KBC_PARITY | KBC_TIMEOUT)) == 0) && (status & KBC_AUX)) {
                byte = data;
            } else {
                flag_mouse = true;
            }
            return;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
}

// sincroniza bytes, primeiro byte é o que tem o FIRST_BYTE ativo
void (sync_bytes)() {
    // 1. control byte
    if ((idx == 0) && (actual_byte & FIRST_BYTE)) { 
        mouse_array[idx] = actual_byte;
        idx++;
    } else if (idx > 0) { // 2. deslocamentos em X e Y
        mouse_array[idx] = actual_byte;
        idx++;
    }
}

void (bytes_to_packet)() {

    for (int i = 0; i < 3; i++) {
        parsing.bytes[i] = mouse_array[i];
    }

    parsing.lb = mouse_array[0] & LB;
    parsing.mb = mouse_array[0] & MB;
    parsing.rb = mouse_array[0] & RB;

    parsing.x_ov = mouse_array[0] & X_OVFL;
    parsing.y_ov = mouse_array[0] & Y_OVFL;

    if (mouse_array[0] & MSB_X_DELTA) {
        parsing.delta_x = 0xFF00 | mouse_array[1];
    } else {
        parsing.delta_x = mouse_array[1];
    }

    if (mouse_array[0] & MSB_Y_DELTA) {
        parsing.delta_x = 0xFF00 | mouse_array[2];
    } else {
        parsing.delta_x = mouse_array[2];
    }
}

int (write_mouse_cmd)(uint8_t cmd) {
    uint8_t res;
    if (kbc_write_cmd(MOUSE_WRITE)) return 1;
    if (kbc_write_arg(cmd)) return 1;
    if (kbc_read_cmd(&res)) return 1;
    return !(res == ACK);
}

int (mouse_disable_reporting)() {
    while (true) {
        if (write_mouse_cmd(DISABLE_DR)) continue;
        break;
    }
    return 0;

}

int (mouse_enable_reporting)() {
    while (true) {
        if (write_mouse_cmd(ENABLE_DR)) continue;
        break;
    }
    return 0;
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}

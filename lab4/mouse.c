#include <lcom/lcf.h>

#include "i8042.h"
#include "mouse.h"

int mouse_hook_id = 3;
uint8_t byte = 0;
bool error = false;

int (mouse_get_conf)(uint8_t *st) {
  // Checking argument validity
  if (st == NULL) return 1;
  // Read status into st
  if (util_sys_inb(KBC_ST_REG, st)) return 1;
  return 0;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1; // ERROR
  *bit_no = BIT(mouse_hook_id); // hook_id = 1 => BIT(1)
  return sys_irqsetpolicy(MOUSE12_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)() {
    uint8_t stat, data;
    error = false;
    for (int i = 0; i < KBC_RETRY_CNT; i++) {
        if (mouse_get_conf(&stat)) continue;
        /* loop while 8042 output buffer is empty */
        if( stat & KBC_OBF ) {
            if (util_sys_inb(KBC_OUT_BUF, &data)) continue;
            if ( ((stat & (KBC_PAR_ERR | KBC_TO_ERR) ) == 0) && (stat & KBC_AUX) ) {
                byte = data;
            }
            else {
                error = true;            
            }
            return;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
}

int (write_mouse_cmd)(uint8_t cmd) {
  if (kbc_write_cmd(MOUSE_WRITE_CMD)) return 1;
  if (kbc_write_argument(cmd)) return 1;
  uint8_t response;
  if (kbc_read_return(&response)) return 1;
  return !(response == MOUSE_ACK);
}

int (mouse_enable_reporting)() {
    while (true) {
        if (write_mouse_cmd(ENBL_DATA_REPORTING)) continue;
        break;
    }
    return 0;
}

int (mouse_disable_reporting)() {
    while (true) {
        if (write_mouse_cmd(DSABL_DATA_REPORTING)) continue;
        break;
    }
    return 0;
}

int16_t (nine_bit_sign_extension) (uint16_t number) {
  return number | ~(BIT(9) - 1);
} 

#include <lcom/lcf.h>

#include "i8042.h"

int hook_id = KEYBOARD1_IRQ;
uint8_t scancode;
bool error = false;

int (keyboard_get_conf)(uint8_t *st) {
  // Checking argument validity
  if (st == NULL) return 1;
  // Read status into st
  if (util_sys_inb(KBC_ST_REG, st)) return 1;
  return 0;
}

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1; // ERROR
  *bit_no = BIT(hook_id); // timer_hook_id = 0 => BIT(0)
  return sys_irqsetpolicy(KEYBOARD1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (kbc_ih)() {
    uint8_t stat, data;
    for (int i = 0; i < KBC_RETRY_CNT; i++) {
        if (keyboard_get_conf(&stat)) continue;
        /* loop while 8042 output buffer is empty */
        if( stat & KBC_OBF ) {
            if (util_sys_inb(KBC_OUT_BUF, &data)) continue;
            if ( (stat & (KBC_PAR_ERR | KBC_TO_ERR)) == 0 ) {
                scancode = data;
                error = false;
            }
            else {
                error = true;
                return;            
            }
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
}

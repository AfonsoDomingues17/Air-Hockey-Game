#include "model.h"

/* Variáveis externas necessárias */
extern unsigned int time_count;
extern uint8_t scancode;

/* Iniciar estado do programa */
MainStateMachine mainState = RUNNING;

/* Timers */
unsigned int idle = 0;

void timer_int() {
    (timer_int_handler)();
    if (time_count % sys_hz() == 0) idle++;
    if (idle == 10) mainState = STOP;
}

void keyboard_int() {
    (kbc_ih)();
    if (scancode == ESC_BREAK_CODE) mainState = STOP;
    idle = time_count;
}

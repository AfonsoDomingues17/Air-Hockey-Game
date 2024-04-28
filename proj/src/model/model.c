#include "model.h"

/* Variáveis externas necessárias */
extern uint8_t scancode;
MainStateMachine mainState = RUNNING; /* Iniciar estado do programa */

void keyboard_int() {
    (kbc_ih)();
    if (scancode == ESC_BREAK_CODE) mainState = STOP;
}

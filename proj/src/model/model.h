#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <lcom/lcf.h>

#include "controller/keyboard/keyboard.h"

typedef enum {
    RUNNING,
    STOP,
} MainStateMachine;

void (keyboard_int)();

#endif

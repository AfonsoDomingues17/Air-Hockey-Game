#include <lcom/lcf.h>

#define BIT(n) (0x1 << (n))

enum l3223_time_units {
l3223_microsecond,
l3223_millisecond,
l3223_second
};

#define TIMER0 0x20
#define TIMER0 0x21
#define TIMER0 0x22

#define CONTROLADOR 0x23
#define STATUS 0x23

//NO MODO ALARME APENAS HÁ 1 INTERRUPT

#define TIMER0_SEL 0
#define TIMER1_SEL BIT(6)
#define TIMER2_SEL BIT(7)

#define PERIODIC 0
#define ALARM BIT(4)

#define MICROSECONDS 0
#define MILLISECONDS BIT(0)
#define SECONDS BIT(1)

//PRIMEIRO ESCREVO A CONTROLL WORD NO CONTROLADOR
//SEGUNDO ESCREVO NO TIMER O LSB E DEPOIS O MSB


#define TIMER0-INT 0
#define TIMER1-INT BIT(1) //ESTES AQUI VERIFICAM SE HOUVE UM INTERRUPT
#define TIMER2-INT BIT(2) //if status & timer0_int e timer == 0 só depois disso posso aplicar a função do prof

//A VARIAVEL DONE PRECISA SER POSTA EM FALSE PRIMEIRO E QUANDO O INTERRUPT FOR REALIZADO PASSAR A TRUE

int set_config(int timer, int interval, enum l3223_time_units unit) {
    if (timer > 2 || timer < 0) return 1;
    uint8_t config;
    config = config | ALARM;

    switch (timer)
    {
    case 0:
        config = config | TIMER0_SEL
        break;
    case 1:
        config = config | TIMER1_SEL
        break;
    case 2:
        config = config | TIMER2_SEL
        break;
    
    default:
        break;
    }

    switch(unit) {
        case l3223_microsecond:
        config = config | MICROSECONDS;
        break;
        case l3223_millisecond:
        config = config | MILLISECONDS;
        break;
        case l3223_second:
        config = config | SECONDS;
        break;
        default:
        break;
    }

    if (sys_outb(CONTROLADOR, config)) return 1;
    if (sys_outb(TIMER0 + timer, interval & 0x00FF)) return 1;
    if (sys_outb(TIMER0 + timer, (interval & 0xFF00) >> 8)) return 1;
    return 0
}

int pp_print_alarm(int timer, int interval, enum l3223_time_units
unit) {
    
}
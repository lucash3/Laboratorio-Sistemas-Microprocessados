#include <msp430.h>
#include <stdbool.h>

#ifndef GPIO_INCLUDED
#define GPIO_INCLUDED

#define LOW 0
#define HIGH 1
#define RED 0
#define GREEN 1
#define TOGGLE 2
#define S1 0
#define S2 1
#define S1ORS2 2
#define S1ANDS2 3
#define UNLOCK_MASTER_PIN PM5CTL0 &= ~LOCKLPM5

void update_button_state(int button, _Bool *button_state, _Bool *memory)
{
    *memory = *button_state;
    switch (button) {
    case S1:
        *button_state = !(P4IN & BIT1);
        break;
    case S2:
        *button_state = !(P2IN & BIT3);
        break;
    case S1ORS2:
        *button_state = !(P4IN & BIT1) || !(P2IN & BIT3);
        break;
    case S1ANDS2:
        *button_state = !(P4IN & BIT1) && !(P2IN & BIT3);
        break;
    }

    return;
}

_Bool led_low(_Bool led)
{
    if (led == RED) return (P1OUT & BIT0);
    else return (P6OUT & BIT6);
}

_Bool button_pressed(_Bool button)
{
    if (button == S1) return !(P4IN & BIT1);
    else return !(P2IN & BIT3);
}

void config_all_buttons()
{
    //S1
    P4DIR &= ~BIT1;           // PXDIR = 0 => entrada
    P4REN |= BIT1;            // Apenas pra entrada
    P4OUT |= BIT1;            // PULL UP

    //S2
    P2DIR &= ~BIT3;           // PXDIR = 0 => entrada
    P2REN |= BIT3;            // Apenas pra entrada
    P2OUT |= BIT3;            // PULL UP

    return;
}

void config_all_leds()
{
    //RED
    P1DIR |= BIT0;              // PXDIR = 1 => saida
    P1OUT &= ~BIT0;             // Começa desligado

    //GREEN
    P6DIR |= BIT6;              // PXDIR = 1 => saida
    P6OUT &= ~BIT6;             // Começa desligado

    return;
}

void change_state_led(_Bool led, int type)
{
    if (led == RED)
    {
        switch (type) {
        case LOW:
            P1OUT &= ~BIT0;
            break;
        case HIGH:
            P1OUT |= BIT0;
            break;
        default:
            P1OUT ^= BIT0;
            break;
        }
    } else {
        switch (type) {
        case LOW:
            P6OUT &= ~BIT6;
            break;
        case HIGH:
            P6OUT |= BIT6;
            break;
        default:
            P6OUT ^= BIT6;
            break;
        }
    }
}

void debounce(int time)
{
     volatile int i;
     for(i = 0; i < time; i++);
}

void debounce_double(double time)
{
     volatile double i;
     for(i = 0; i < time; i++);
}
#endif

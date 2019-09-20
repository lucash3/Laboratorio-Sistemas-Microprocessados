#include <msp430.h>
#include <stdint.h>

void debounce (volatile uint16_t dt) {
    while(dt--);
    return;
}


/**
 * main.c
 */
int main(void)
{
    PM5CTL0 &= ~(LOCKLPM5);
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P4DIR &= ~(BIT1);           // habilita entrada no P4.1 (botao)
    P4REN |= BIT1;              // habilita resistor
    P4OUT |= BIT1;              // escolhe resistor de pull up

    P1DIR |= BIT0;              // habilita saida no P1.0 (LED)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    while (1) {
        while (P4IN & BIT1) {
            P1OUT &= ~(BIT0);
        }
        debounce(10000);

        while (!(P4IN & BIT1)) {
            P1OUT |= BIT0;
        }
        debounce(10000);
    }

    return 0;
}

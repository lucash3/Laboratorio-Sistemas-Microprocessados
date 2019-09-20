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

    P2DIR &= ~(BIT3);           // habilita entrada no P2.3 (botao)
    P2REN |= BIT3;              // habilita resistor
    P2OUT |= BIT3;              // escolhe resistor de pull up

    P1DIR |= BIT0;              // habilita saida no P1.0 (LED)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    P6DIR |= BIT6;              // habilita saida no P6.6 (LED)
    P6REN &= ~(BIT6);           // habilita resistor de pull up
    P6OUT &= ~(BIT6);           // zera saida

    while (1) {
        if (!(P4IN & BIT1) || !(P2IN & BIT3)) {     // se algum botao for press.
            debounce(5000);     // tempo de setup (poderia usar sleep aqui)
            if (!(P4IN & BIT1) && !(P2IN & BIT3)) { // S1 && S2 press.
                //desliga LEDs
                P1OUT &= ~(BIT0);
                P6OUT &= ~(BIT6);
            } else {
                if (!(P4IN & BIT1) && (P2IN & BIT3)) {  // S1 press. S2 solto
                    // inverte ambos os LEDs
                    P1OUT ^= BIT0;
                    P6OUT ^= BIT6;
                }
                if (!(P2IN & BIT3) && (P4IN & BIT1)) {  // S2 press. S1 solto
                    // inverte LED verde
                    P6OUT ^= BIT6;
                }
            }
            debounce(25000);
        }
    }
    return 0;
}

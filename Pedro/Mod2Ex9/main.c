#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    /* Config. */
    P1OUT &= ~BIT0;             // led apagado inicialmente
    P1DIR |= BIT0;              // habilita saida em P1.0

    // Config. timer
    TB0CTL = (TBSSEL__ACLK  |   // seleciona ACLK
              MC__UP        |   // modo de contagem UP
              TBCLR);           // comecando do zero

    TB0CCR0 = 0x4000 - 1;       // conta 0.5s

    /* Loop */
    while (1) {
        while (!(TB0CCTL0 & CCIFG));        // aguarda contador chegar em CCR0
        TB0CCTL0 &= ~CCIFG;                 // limpa flag CCIFG

        P1OUT ^= BIT0;                      // alterna estado do led
    }

    return 0;
}

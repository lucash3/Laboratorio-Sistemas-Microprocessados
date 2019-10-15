#include <msp430.h>
#include <stdint.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    P1DIR |= BIT0;

    // Config. timer
    TB0CTL = (TBSSEL__SMCLK |   // seleciona SMCLK @1048576Hz
              MC__UP        |   // modo de contagem UP
              TBCLR);           // comecando do zero

    TB0CCTL0 = CCIE;
    TB0CCTL1 = CCIE;

    __enable_interrupt();

    TB0CCR0 = (1048576/50) - 1;
    TB0CCR1 = (1048576/100) - 1;

    while (1);

    return 0;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCR0_ISR() {
    P1OUT ^= BIT0;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_CCR1_ISR() {
    P1OUT ^= BIT0;
    TB0CCTL1 &= ~(CCIFG);
}


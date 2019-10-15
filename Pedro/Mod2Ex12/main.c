#include <msp430.h>
#include <stdint.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    P1DIR |= BIT0;
    P6DIR |= BIT6;

    // Config. timer
    TB0CTL = (TBSSEL__ACLK  |   // seleciona ACLK @32768Hz
              MC__UP        |   // modo de contagem UP
              TBCLR);           // comecando do zero

    TB0CCTL0 = CCIE;

    TB1CTL = (TBSSEL__ACLK  |   // seleciona ACLK @32768Hz
              MC__UP        |   // modo de contagem UP
              TBCLR);           // comecando do zero

    TB1CCTL0 = CCIE;
    __enable_interrupt();

    TB0CCR0 = 0x2000 - 1;       // 0x2000 = 32768/(2*2)
    TB1CCR0 = 0x0CCD - 1;       // 0x0CCD ~ 32768/(5*2)

    while (1);

    return 0;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCR0_ISR() {
    P1OUT ^= BIT0;
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void TB1_CCR0_ISR() {
    P6OUT ^= BIT6;
}

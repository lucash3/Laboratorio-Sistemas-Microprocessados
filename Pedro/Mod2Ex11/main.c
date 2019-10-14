#include <msp430.h>
#include <stdint.h>

unsigned char flag = 0;

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
    TB0CCTL1 = CCIE;
    __enable_interrupt();

    TB0CCR0 = 0x4000 - 1;       // 0x8000 = 32768
    // Conta 0,5 segundo

    TB0CCR1 = 0x2000 - 1;

    while (1);

    return 0;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCR0_ISR() {
    P1OUT ^= BIT0;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_CCR1_ISR() {
    TB0CCTL1 &= ~(CCIFG);

    if (flag) {
        flag = 0;
        TB0CCR1 = 0x2000 - 1;
    } else {
        flag = 1;
        TB0CCR1 = 0x4000 - 1;
    }
    P6OUT ^= BIT6;
}

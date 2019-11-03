#include <msp430.h> 
//#include <stdint.h>


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1OUT &= ~BIT0;
    P1DIR |= BIT0;

    TB0CTL = (TBSSEL__ACLK | ID__4 | MC__CONTINOUS | TBCLR);

    // continuous conta 8 segundos

    // up conta 4 segundos

    TB0CCR0 = 0x8000 - 1;
    // 32768 batidas = teto


    while(1){
        while(!(TB0CCTL0 & CCIFG));
        TB0CCTL0 &= ~CCIFG;

        P1OUT ^= BIT0;
    }


    return 0;
}

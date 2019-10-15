#include <msp430.h>
#include <stdint.h>

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCR0_ISR() {
    P1OUT ^= BIT0;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_CCR1_ISR() {
    P1OUT ^= BIT0;
    TB0CCTL1 &= ~(CCIFG);
}

int main(void) {
    unsigned int duty_cycle = 50;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    // Config. LED
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // Config. S2
    P2DIR &= ~BIT3;
    P2REN |= BIT3;
    P2OUT &= ~ BIT3;

    // Config. S1
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT &= ~BIT1;

    // Config. timer
    TB0CTL = (TBSSEL__SMCLK |   // seleciona SMCLK @1048576Hz
              MC__UP        |   // modo de contagem UP
              TBCLR);           // comecando do zero

    TB0CCTL0 = CCIE;
    TB0CCTL1 = CCIE;

    __enable_interrupt();

    TB0CCR0 = (1048576/50) - 1;
    TB0CCR1 = ((1048576/50) * (duty_cycle/100)) - 1;

    while (1) {
        if (!(P4IN & BIT1) || !(P2IN & BIT3)) {     // se algum botao for press.
            if (!(P4IN & BIT1) && (P2IN & BIT3)) {  // S1 press. S2 solto
                // decrementa duty cycle
                if (duty_cycle == 0);
                else duty_cycle -= 10;
            }
            if (!(P2IN & BIT3) && (P4IN & BIT1)) {  // S2 press. S1 solto
                // incrementa duty cycle
                if (duty_cycle == 100);
                else duty_cycle += 10;
            }
            TB0CCR1 = ((1048576/50) * (duty_cycle/100)) - 1;
        }
    }

    return 0;
}

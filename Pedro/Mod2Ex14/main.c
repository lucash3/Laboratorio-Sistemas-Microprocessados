#include <msp430.h>
#include <stdint.h>

void debounce (volatile uint16_t dt) {
    while(dt--);
    return;
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

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    // Config. LED
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // Config. S2
    P2DIR &= ~BIT3;
    P2REN |= BIT3;
    P2OUT |= BIT3;

    // Config. S1
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1;

    // Config. timer
    TB0CTL = (TBSSEL__ACLK  |   // seleciona ACLK @32768Hz
              MC__UP        |   // modo de contagem UP
              TBCLR);           // comecando do zero

    TB0CCTL0 = CCIE;
    TB0CCTL1 = CCIE;

    __enable_interrupt();

    TB0CCR0 = 655 - 1;          // 655 = 32768/50
    TB0CCR1 = 327 - 1;          // 327 = 655/2

    while (1) {
        while ((P4IN & BIT1) && (P2IN & BIT3));     // espera botao ser press.
        debounce(5000);
        if ((P4IN & BIT1) && !(P2IN & BIT3)) {  // S1 press. S2 solto
            // decrementa duty cycle
            TB0CCR1 -= 65;      // 65 = 10% duty cycle
        }
        if ((P2IN & BIT3) && !(P4IN & BIT1)) {  // S2 press. S1 solto
            // incrementa duty cycle
            TB0CCR1 += 65;
        }

        while (!(P4IN & BIT1) || !(P2IN & BIT3));     // espera botao ser solto
        debounce(5000);
    }

    return 0;
}

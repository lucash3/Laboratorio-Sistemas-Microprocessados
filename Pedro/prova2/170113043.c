#include <msp430.h>
#include <stdint.h>

/*
 * LED vermelho: T = 2s; f = 0.5Hz
 * LED verde:    T = 4s; f = 0.25Hz
 * Duty cycle:   50%
 * Botão S1:     Pausa funcionamento
 */

void debounce (volatile uint16_t dt) {
    while(dt--);
    return;
}

unsigned char flag = 0;

int main(void) {
    int debug = 0;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    // Config botao
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1;

    // Config. LEDs
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P6DIR |= BIT6;
    P6OUT &= ~BIT6;

    // Config. timer
    TB0CTL = (TBSSEL__ACLK   |  // seleciona ACLK @32768Hz
              MC__UP         |  // modo de contagem UP
              TBCLR);           // comecando do zero

    TB0CCTL0 = CCIE;

    __enable_interrupt();

    TB0CCR0 = 0x8000 - 1;       // 0x8000 = 32768

    while (1) {
        while (P4IN & BIT1);    // espera botao ser apertado
        debounce(50000);
        debug++;

        // pausa timer, reinicia LEDs
        TB0CTL = TBCLR;

        while (P4IN & BIT1);    // espera botao ser apertado
        debounce(50000);
        debug++;

        // inicia timer
        TB0CTL = (TBSSEL__ACLK   |  // seleciona ACLK @32768Hz
                  MC__UP         |  // modo de contagem UP
                  TBCLR);           // comecando do zero

    }

    return 0;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCR0_ISR() {

    if (!flag) flag = 1;
    else {
        P6OUT ^= BIT6;
        flag = 0;
    }
    P1OUT ^= BIT0;
}

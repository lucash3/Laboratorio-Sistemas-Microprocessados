#include <msp430.h>
#include <stdint.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    /* Config. */
    P1OUT |= BIT0;             // LED apagado inicialmente
    P1DIR |= BIT0;              // habilita saida em P1.0 (LED)

    P6OUT &= ~BIT6;             // LED apagado inicialmente
    P6DIR |= BIT6;              // habilita saída em P6.6 (LED)

    // Config. timer
    TB0CTL = (TBSSEL__ACLK  |   // seleciona ACLK @32768Hz
              MC__UP        |   // modo de contagem UP
              TBCLR);           // comecando do zero

    TB0CCR0 = 0x4000 - 1;       // 0x8000 = 32768
    // Conta 1 segundo

    int counter = 0;

    /* Loop */
    while (1) {
        while (!(TB0CCTL0 & CCIFG));        // aguarda contador chegar em CCR0
        TB0CCTL0 &= ~CCIFG;                 // limpa flag CCIFG

        P1OUT ^= BIT0;                      // alterna estado do LED vermelho
        counter++;

        if (P6OUT & BIT6) P6OUT &= ~BIT6;   // apaga LED verde

        if (counter == 10) {                // conta 5s
            counter = 0;
            P6OUT |= BIT6;                  // liga o LED verde
        }
    }

    return 0;
}

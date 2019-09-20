#include <msp430.h>
#include <stdint.h>

void debounce (volatile uint16_t dt) {
    while(dt--);
    return;
}

unsigned char rand() {
    static unsigned char num = 5;
    num = (num * 17) % 7;
    return num;
}

int main(void)
{
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

    unsigned char number = rand();    // ambos limitados entre 1 e 6
    unsigned char counter = 0;

    while (1) {
        if (!(P4IN & BIT1) || !(P2IN & BIT3)) {     // se algum botao for press.
            if (!(P4IN & BIT1) && (P2IN & BIT3)) {  // S1 press. S2 solto
                // incrementa resposta
                if (counter == 6);
                else counter++;
            }
            if (!(P2IN & BIT3) && (P4IN & BIT1)     // S2 press. S1 solto
                && counter > 0) {
                if (counter == number) {
                    // liga LED verde
                    P6OUT |= BIT6;
                    P1OUT &= ~(BIT0);
                } else {
                    // liga LED vermelho
                    P1OUT |= BIT0;
                    P6OUT &= ~(BIT6);
                }
                number = rand();        // novo numero
                counter = 0;            // reseta resposta
            }
            debounce(50000);
        }
    }

    return 0;
}

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
    PM5CTL0 &= ~(LOCKLPM5);

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

    uint8_t number = rand();    // limitado entre 1 e 6
    uint8_t counter = 0;        // limitado entre 1 e 6
    uint8_t S1press = 0;        // flag
    uint8_t S2press = 0;        // flag

    while (1) {
        if (!(P4IN & BIT1) || !(P2IN & BIT3)) {     // se algum botao for press.
            debounce(5000);                         // tempo de setup

            if (!(P4IN & BIT1) &&
                 (P2IN & BIT3)) {                   // S1 press. S2 solto

                S1press = 1;

            } else if (!(P2IN & BIT3) &&
                        (P4IN & BIT1) &&            // S2 press. S1 solto
                        counter > 0) {

                S2press = 1;

            }

            if (S1press) {
                // incrementa resposta
                if (counter == 6);
                else counter++;

                // reseta flag
                S1press = 0;
            }

            if (S2press) {
                if (counter == number) {
                    // acerto -> liga LED verde
                    P6OUT |= BIT6;
                    P1OUT &= ~(BIT0);
                } else {
                    // erro -> liga LED vermelho
                    P1OUT |= BIT0;
                    P6OUT &= ~(BIT6);
                }
                number = rand();        // novo numero
                counter = 0;            // reseta resposta
                S2press = 0;            // reseta flag
            }
            debounce(25000);
        }
    }

    return 0;
}

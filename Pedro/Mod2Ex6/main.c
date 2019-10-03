#include <msp430.h>
#include <stdint.h>

void debounce (volatile uint16_t dt) {
    while(dt--);
    return;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);

    P4DIR &= ~BIT1;             // habilita entrada no P4.1 (botao)
    P4REN |= BIT1;              // habilita resistor
    P4OUT |= BIT1;              // escolhe resistor de pull up

    P2DIR &= ~BIT3;             // habilita entrada no P2.3 (botao)
    P2REN |= BIT3;              // habilita resistor
    P2OUT |= BIT3;              // escolhe resistor de pull up

    P1DIR |= BIT0;              // habilita saida no P1.0 (LED)
    P1REN &= ~BIT0;             // habilita resistor de pull up
    P1OUT &= ~BIT0;             // zera saida

    P6DIR |= BIT6;              // habilita saida no P6.6 (LED)
    P6REN &= ~BIT6;             // habilita resistor de pull up
    P6OUT &= ~BIT6;             // zera saida

    uint8_t state = 0;

    while (1) {

        // ----------------- MUDANCA DE ESTADO ---------------------------------

        // Pressionar ambos os botoes nao altera o estado do contador

        if (!(P4IN & BIT1) || !(P2IN & BIT3)) {     // se algum botao for press.
            debounce(5000);     // tempo de setup (poderia usar sleep aqui)
            if (!(P4IN & BIT1) && (P2IN & BIT3)) {  // S1 press. S2 solto
                // decrementa estado
                if (state == 0) state = 3;
                else state--;
            }
            if (!(P2IN & BIT3) && (P4IN & BIT1)) {  // S2 press. S1 solto
                // incrementa estado
                if (state == 3) state = 0;
                else state++;
            }
            debounce(25000);    // tratamento de debounce
        }

        // ------------------ APLICACAO DE ESTADO ------------------------------

        // LEDs: Red    ->  P1.0
        //       Green  ->  P6.6

        switch (state) {
            case 0:
                // RG = 00
                P1OUT &= ~BIT0;
                P6OUT &= ~BIT6;
                break;

            case 1:
                // RG = 01
                P1OUT &= ~BIT0;
                P6OUT |= BIT6;
                break;

            case 2:
                // RG = 10
                P1OUT |= BIT0;
                P6OUT &= ~BIT6;
                break;

            case 3:
                // RG = 11
                P1OUT |= BIT0;
                P6OUT |= BIT6;
                break;
        }
    }

    return 0;
}

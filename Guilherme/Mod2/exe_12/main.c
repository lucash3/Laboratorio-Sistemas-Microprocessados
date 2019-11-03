#include <msp430.h> 

// PRAGMA VECTOR AGRUPADO COM INSTÂNCIAS DIFERENTES

/**
 * main.c
 */
int main(void)
{

    PM5CTL0 &= ~LOCKLPM5;       // destrava pinos

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    P6DIR |= BIT6;              // habilita saida no P6.6 (LED VERDE)
    P6REN &= ~(BIT6);           // habilita resistor de pull up
    P6OUT &= ~(BIT6);           // zera saida

    TB0CTL = (TBSSEL__ACLK |        // clk @32768hz
                      TBIE |        // habilita interrupcao local de teto de contagem (habilita TBIFG)
                    MC__UP |        // MC__UP faz teto de contagem (conta, chega no teto, volta, repete)
                    TBCLR);         // começa do zero

    TB0CCR0 = 0x8000 - 1;    // @2hz
    TB0CCR1 = 0x199A - 1;    // @5hz

    TBCCTL1 = CCIE;

    __enable_interrupt();  // cntrl + space, HABILITADOR GLOBAL DE INTERRUPÇÃO EM SR
                            // SETA GIE no SR

    while(1){

    }

    return 0;
}

// INTERRUPCAO AGRUPADA
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_ISR(){

    switch(TB0IV){          // interrupt vector (TB0IFG é zerada na leitura)
        case 0x2:  // canal CCR1
            P6OUT ^= BIT6;
            break;

        case 0x4:  // canal 2

            break;

        case 0xE: // canal do overflow do timer
            P1OUT ^= BIT0;                      // executado qnd led chega a 1 segundo
            break;

        default:
            break;
    }

}

#include <msp430.h> 

// PRAGMA VECTOR INTERRUPÇÃO AGRUPADA
// o led pisca exatamente 1hz (0,5 segundos apagado e 0,5 aceso)

/**
 * main.c
 */
int main(void)
{

    P1OUT &= ~BIT0;
    P1DIR |= BIT0;
    PM5CTL0 &= ~LOCKLPM5;       // destrava pinos

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    TB0CTL = (TBSSEL__ACLK |        // clk @32768hz
                      TBIE |        // habilita interrupcao local de teto de contagem (habilita TBIFG)
                    MC__UP |        // MC__UP faz teto de contagem (conta, chega no teto, volta, repete)
                    TBCLR);         // começa do zero

    TB0CCR0 = 0x8000 - 1;   //ACLK conta 2 segundos, pegamos metade

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

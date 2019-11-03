#include <msp430.h> 

//PWM DUTY CYCLE FIXO

/**
 * main.c
 */
void main()
{

    PM5CTL0 &= ~LOCKLPM5;       // destrava pinos

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    TB0CTL = (TBSSEL__ACLK |        //
                      TBIE |        // habilita interrupcao local de teto de contagem (habilita TBIFG)
                    MC__UP |        // MC__UP faz teto de contagem (conta, chega no teto, volta, repete)
                    TBCLR);         // começa do zero

    TB0CCTL1 = CCIE;

    TB0CCR0 = 0x28F - 1;
    TB0CCR1 = 0x147 - 1;

    __enable_interrupt();  // cntrl + space, HABILITADOR GLOBAL DE INTERRUPÇÃO EM SR
                            // SETA GIE no SR

    while(1){

    }


}

// INTERRUPCAO AGRUPADA
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_CCR1_ISR(){

    switch(TB0IV){          // interrupt vector (TB0IFG é zerada na leitura)
        case 0x2:  // canal CCR1
            P1OUT &= ~BIT0;
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

#include <msp430.h> 

//PWM DUTY CYCLE VARIÁVEL

void debounce(int input){

    volatile unsigned int dt;
    dt = input;
    while(dt--);

}

/**
 * main.c
 */
void main()
{

    PM5CTL0 &= ~LOCKLPM5;       // destrava pinos

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P4DIR &= ~(BIT1);           // habilita entrada no P4.1 (botão S1)
    P4REN |= BIT1;              // habilita resistor
    P4OUT |= BIT1;              // escolhe resistor de pull up

    P2DIR &= ~(BIT3);           // habilita entrada no P2.3 (botão S2)
    P2REN |= BIT3;              // habilita resistor
    P2OUT |= BIT3;              // escolhe resistor de pull up


    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    TB0CTL = (TBSSEL__ACLK |        //
                      TBIE |        // habilita interrupcao local de teto de contagem (habilita TBIFG)
                    MC__UP |        // MC__UP faz teto de contagem (conta, chega no teto, volta, repete)
                    TBCLR);         // começa do zero

    TB0CCTL1 = CCIE;


    TB0CCR0 = 655 - 1;     // 50hz
    TB0CCR1 = 655 * 0.5;     // duty cycle

    __enable_interrupt();  // cntrl + space, HABILITADOR GLOBAL DE INTERRUPÇÃO EM SR
                            // SETA GIE no SR

    while(1){

        while((P2IN & BIT3) && (P4IN & BIT1));  // não faz nada se não apertamos nada

        debounce(10000);

        if(!(P4IN & BIT1)){    // aperta s1
            debounce(10000);
            TB0CCR1 -= 0.1 * TB0CCR0;
        }

        if(!(P2IN & BIT3)){    // aperta s2
            debounce(10000);
            TB0CCR1 += 0.1 * TB0CCR0;
        }


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

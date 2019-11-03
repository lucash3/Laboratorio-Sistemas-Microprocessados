#include <msp430.h> 


/*
 * SMCLK (@1048576hz) conta 65ms, o ID deixa x vezes mais lento
 * ACLK (@32768hz) conta 2 segundos
 *
 */

/**
 * main.c
 */
int main(void)
{

    PM5CTL0 &= ~LOCKLPM5;       // destrava pinos

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    // setup padrão pra PWM
    // --------------------------------------------

    TB0CTL = (TBSSEL__ACLK |        //
                      TBIE |        // habilita interrupcao local de teto de contagem (habilita TBIFG)
                    MC__UP |        // MC__UP faz teto de contagem (conta, chega no teto, volta, repete)
                    TBCLR);         // começa do zero

    TB0CCTL1 = CCIE;  // habiliato pausa

    //-----------------------------------------------

    // N0 = Tempo/Tclk = (1 / X hz desejados) / (1 / x_CLK)

    volatile unsigned long long int batidas_pwm;
    batidas_pwm = 655;

    volatile unsigned int duty_cycle;
    duty_cycle = 0;

    duty_cycle = (0.5 * batidas_pwm);   // 50%

    TB0CCR0 = batidas_pwm;
    TB0CCR1 = duty_cycle;

    __enable_interrupt();  // cntrl + space, HABILITADOR GLOBAL DE INTERRUPÇÃO EM SR
                            // SETA GIE no SR

    while(1){

    }

	return 0;
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

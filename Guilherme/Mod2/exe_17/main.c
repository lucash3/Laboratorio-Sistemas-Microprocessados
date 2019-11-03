#include <msp430.h> 
#include <stdint.h>

// CAPTURA DE BOTÕES (TEMPO EM MILISSEGUNDOS)

/*
 * SMCLK (@1048576hz) conta 65ms, o ID deixa x vezes mais lento
 * ACLK (@32768hz) conta 2 segundos
 *
 */

void debounce(uint16_t input){

    volatile uint16_t dt;
    dt = input;
    while(dt--);

}

/**
 * main.c
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~(LOCKLPM5);
	
    P4DIR &= ~(BIT1);           // habilita entrada no P4.1 (botão S1)
    P4REN |= BIT1;              // habilita resistor
    P4OUT |= BIT1;              // escolhe resistor de pull up

    P2DIR &= ~(BIT3);           // habilita entrada no P2.3 (botão S2)
    P2REN |= BIT3;              // habilita resistor
    P2OUT |= BIT3;              // escolhe resistor de pull up

    P1DIR |= BIT0;              // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida

    P6DIR |= BIT6;              // habilita saida no P6.6 (LED VERDE)
    P6REN &= ~(BIT6);           // habilita resistor de pull up
    P6OUT &= ~(BIT6);           // zera saida


    volatile unsigned long int tempo;
    volatile unsigned long int tempo_final;

    while(1){

        tempo = 0;
        tempo_final = 0;

        while(P4IN & BIT1);  // não faz nada se não apertamos S1

        TB0CTL = (TBSSEL__ACLK | ID__4 | MC__CONTINOUS | TBCLR);

        // ACLK conta 2 segundos
        // ID__4 faz o contador ficar 4 vezes mais lento, 8 segundos

       // aperta s2, espera
       while(P2IN & BIT3);    //não faz nada enquanto não se aperta s2

       tempo = TB0R;    // batidas de clock
       tempo_final = ((tempo * 1000) / 8192);      //8192 = ACLK / 4

       if(tempo_final >= 3000){
           P1OUT &= ~(BIT0);
           P6OUT |= (BIT6);
       }
       if((tempo_final < 3000) && (tempo_final > 1000)){
           P1OUT |= (BIT0);
           P6OUT |= (BIT6);
       }
       if(tempo_final < 1000){
           P1OUT |= (BIT0);
           P6OUT &= ~BIT6;
       }

       while(1);

    }


    return 0;

}

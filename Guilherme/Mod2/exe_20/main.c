// trena digital com sensor

#include <msp430.h> 
#include <stdint.h>

void wait(uint16_t input){

    volatile uint16_t dt;
    dt = input;

    TB0CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TB0CCR0 = dt;

    while(!(TB0CCTL0 & CCIFG));
    TA4CCTL0 &= ~CCIFG;


}

/**
 * main.c
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
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

    //S0                        // envia pro ultrassom no P6.0
    P6DIR |= BIT0;
    P6OUT |= BIT0;

    // entrada ultrassom
    P6DIR &= ~(BIT1);           // entrada
    P6REN |= BIT1;              // habilitamos sua leitura
    P6OUT |= BIT1;              // inicializamos com zero
    P6SEL1 |= (BIT1);


    volatile unsigned int tempo;
    volatile unsigned int tempo2;
    volatile unsigned int tempo_final;

    while(1){

        tempo = 0;
        while(P4IN & BIT1);  // não faz nada se não apertamos S1

        P6OUT |= BIT0;
        wait(20);
        P6OUT &= ~BIT0;

        while(!(P6IN & BIT1));      //espero meu pino conectado ao echo receber um sinal

        TB0CTL = (TBSSEL__SMCLK | MC__CONTINOUS | TBCLR);

        while(P6IN & BIT1);     // espero parar de receber um sinal

        tempo = TB0R;    // batidas de clock


        while(1);   // no lugar disso aqui eu ligaria os leds de acordo e tal

       //TB0CCTL1 &= ~CCIFG;

    }

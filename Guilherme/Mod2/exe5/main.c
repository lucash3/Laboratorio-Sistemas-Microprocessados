#include <msp430.h> 
#include <stdint.h>

// muda os dois leds ao apertar s1, alterna o verde ao apertar s2, apaga caso ambos botões sejam pressionados

void debounce(){

    volatile uint16_t dt;
    dt = 25000;
    while(dt--);

}


int main(void)
{
    PM5CTL0 &= ~(LOCKLPM5);
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

    P6DIR |= BIT6;              // habilita saida no P1.0 (LED VERDE)
    P6REN &= ~(BIT6);           // habilita resistor de pull up
    P6OUT &= ~(BIT6);


    while(1){

        while((P2IN & BIT3) && (P4IN & BIT1));

        if(!(P4IN & BIT1)){
            debounce();
            P6OUT ^= BIT6;
            P1OUT ^= BIT0;
        }

        if(!(P2IN & BIT3)){
            debounce();
            P6OUT ^= BIT6;
        }

        debounce();

        if(!(P2IN & BIT3) && !(P4IN & BIT1)){

            debounce();
            P6OUT &= ~(BIT6);
            P1OUT &= ~(BIT0);
        }


    }


    return 0;
}

#include <msp430.h> 
#include <stdint.h>

// aperta s1 ou s2, muda estado do led

void debounce(){

    volatile uint16_t dt;
    dt = 25000;
    while(dt--);

}


int main(void)
{
    PM5CTL0 &= ~(LOCKLPM5);
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P4DIR &= ~(BIT1);           // habilita entrada no P4.1 (botao)
    P4REN |= BIT1;              // habilita resistor
    P4OUT |= BIT1;              // escolhe resistor de pull up

    P2DIR &= ~(BIT3);           // habilita entrada no P2.3
    P2REN |= BIT3;              // habilita resistor
    P2OUT |= BIT3;              // escolhe resistor de pull up

    P1DIR |= BIT0;              // habilita saida no P1.0 (LED)
    P1REN &= ~(BIT0);           // habilita resistor de pull up
    P1OUT &= ~(BIT0);           // zera saida


    while(1){

        while((P2IN & BIT3) && (P4IN & BIT1));

        if(!(P2IN & BIT3) || !(P4IN & BIT1)){
            debounce();
            P1OUT ^= BIT0;
        }

        while(!(P2IN & BIT3) && !(P4IN & BIT1));

        debounce();

    }


	return 0;
}

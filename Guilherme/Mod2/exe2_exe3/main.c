#include <msp430.h> 
#include <stdint.h>

// aperta s1 muda estado do led c/ debounce

void debounce(){

    volatile uint16_t dt;
    dt = 23000;
    while(dt--);

}

/**
 * main.c
 */
int main(void)
{
    PM5CTL0 &= ~(LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    P4DIR  &= ~(BIT1);          // botão 4.1
    P4REN  |= BIT1;
    P4OUT  |= BIT1;

    P1DIR  |= BIT0;             // led
    P1OUT  &= ~(BIT0);
    P1REN &= ~(BIT0);

    while(1){

        while(P4IN & BIT1);

        if(!(P4IN & BIT1)){
            debounce();
            P1OUT ^= BIT0;
        }

        while(!(P4IN & BIT1));

        debounce();

    }

	return 0;
}

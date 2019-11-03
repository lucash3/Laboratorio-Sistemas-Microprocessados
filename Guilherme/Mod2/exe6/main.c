#include <msp430.h> 
#include <stdint.h>

// máquina de estados, contador binário

void debounce(uint16_t input){

    volatile uint16_t dt;
    dt = input;
    while(dt--);

}

unsigned char rand(){
    static unsigned char num = 5;
    num = (num * 17) % 7;
    return num;

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

    uint16_t contador;
    contador = 0;         // contador do usuario

    // o jogo começa com as luzes apagadas
    // idéia: aperte 7 vezes para terminar o jogo!

    while(1){

        while((P2IN & BIT3) && (P4IN & BIT1));  // não faz nada se não apertamos nada

        if(!(P4IN & BIT1)){     // aperta s1
            debounce(25000);
            if(contador > 0){
                contador--;
            }
        }

        if(!(P2IN & BIT3)){    // aperta s2
            debounce(25000);
            if(contador < 3){
                contador++;
            }
        }

        if(contador == 0){
            P6OUT &= ~(BIT6);
            P1OUT &= ~(BIT0);
        }
        if(contador == 1){
            P6OUT |= BIT6;
            P1OUT &= ~(BIT0);
        }
        if(contador == 2){
            P6OUT &= ~(BIT6);
            P1OUT |= BIT0;
        }
        if(contador == 3){
            P6OUT |= BIT6;
            P1OUT |= BIT0;
        }

    }


    return 0;
}

#include <msp430.h> 

// Aluno: Guilherme Braga Pinto, 17/0162290
// Turma D, Professor Daniel Café
// Prova 2 de Lab. SisMic

/*  Timers!
 *
 *  SMCLK (@1048576hz) conta 65ms, o ID deixa x vezes mais lento
 *  ACLK (@32768hz) conta 2 segundos
 */

inline void trava_milissegundos(long long int milissegundos){

    // função conta até 8 segundos
    // recebo um long int

    // 8192 = ACLK / 4
    // milisegundos = ((batidas de clock * 1000) / 8192)
    // batidas de clock = ((milissegundos * 8192) / 1000)
    // o nosso CCR0!

    volatile unsigned long int batidas_de_clock;
    batidas_de_clock = (milissegundos * 8.192);

    TB0CTL = (TBSSEL__ACLK | ID__4 | MC__CONTINOUS | TBCLR);
    TB0CCR0 = batidas_de_clock - 1;

    while(!(TB0CCTL0 & CCIFG));
    TB0CCTL0 &= ~CCIFG;

}

void debounce(int input){

    // esperamos para ler o botão
    volatile int dt;
    dt = input;
    while(dt--);

}

void main(void)
{

    PM5CTL0 &= ~LOCKLPM5;                   // destrava pinos

    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer

    P4DIR &= ~(BIT1);                       // habilita entrada no P4.1 (botão S1)
    P4REN |= BIT1;                          // habilita resistor
    P4OUT |= BIT1;                          // escolhe resistor de pull up

    P1DIR |= BIT0;                          // habilita saida no P1.0 (LED VERMELHO)
    P1REN &= ~(BIT0);                       // habilita resistor de pull up
    P1OUT &= ~(BIT0);                       // zera saida

    P6DIR |= BIT6;                          // habilita saida no P6.6 (LED VERDE)
    P6REN &= ~(BIT6);                       // habilita resistor de pull up
    P6OUT &= ~(BIT6);                       // zera saida


    volatile unsigned int rodando;          // flag de rodar lógica de leds
    rodando = 0;                            // começa falsa

    while(1){

        if((P4IN & BIT1) && (rodando == 0)){
                                            // se não esta rodando, trava
            debounce(10000);
            while(P4IN & BIT1);
        }

        if((!(P4IN & BIT1)) && (rodando == 0)){
            debounce(10000);                // mudamos o estado da flag de rodar o programa
            rodando = 1;
        }

        if((!(P4IN & BIT1)) && (rodando == 1)){
            debounce(10000);                // mudamos o estado da flag de rodar o programa
            rodando = 0;
        }
        if (rodando == 1){

            // lógica de ligar e apagar leds
            P1OUT ^= (BIT0);
            P6OUT ^= (BIT6);
            trava_milissegundos(1000);      // o vermelho fica 1 segundo aceso e 1 apagado
            P1OUT ^= (BIT0);
            trava_milissegundos(1000);
            P6OUT ^= (BIT6);                // o verde fica ligado por 2 segundos
            P1OUT ^= (BIT0);
            trava_milissegundos (1000);
            P1OUT ^= (BIT0);
            trava_milissegundos (1000);
                                            // a pausa da lógica funciona melhor segurando o botão ao final da lógica dos leds

        }

    }

}



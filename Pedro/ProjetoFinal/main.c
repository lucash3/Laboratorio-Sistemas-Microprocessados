#include <msp430.h> 
//#include "../biblioteca/LCD_base.c"
#include "../biblioteca/gpio.c"
#include "../biblioteca/clock.c"
#include "../biblioteca/buzzer.c"

volatile int durationFG = 0;

// nota em Hz, duracao em segundos
void play(int note, float duration) {

    TB0CCR0 = get_N(note, FREQUENCY, MY_SMCLK);
    if (note == 0) {
        TB0CCR0 = 0;
        P5OUT &= ~BIT4;
    } else {
        P5OUT |= BIT4;
    }
    TB0CTL |= TBSSEL__SMCLK;

    TB1CCR0 = get_N(duration, PERIOD, MY_ACLK);
    durationFG = 0;
    TB1CTL |= TBSSEL__ACLK;

    while(durationFG == 0);

    return;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    UNLOCK_MASTER_PIN;

    config_all_buttons();
    config_all_leds();

    _Bool fechado = false;          // Aberto por padrão
    _Bool memoria = fechado;

    TB0CTL  = TBSSEL__SMCLK |        // Configura SMCLK @1048576 Hz
                  MC__UP       |        // Modo de contagem UP
                  TBIE         |        // Habilita interrupção de overflow
                  TBCLR;                // Começa do zero

    TB1CTL  = TBSSEL__ACLK |        // Configura ACLK @32768 Hz
                      MC__UP       |        // Modo de contagem UP
                      TBIE         |        // Habilita interrupção de overflow
                      TBCLR;                // Começa do zero

    TB0CCTL1 = CCIE;                // Habilita interrupção localmente Do canal 1 (permite ver a flag ccifg)
    TB1CCTL1 = CCIE;

    P1DIR |= BIT1;              // PXDIR = 1 => saida
    P1REN &= ~BIT1;
    P1OUT &= ~BIT1;

    P5DIR |= BIT4;
    P5REN &= ~BIT4;
    P5OUT &= ~BIT4;

    __enable_interrupt();           // Seta GIE no registro SR

    while(1)
    {
        play(notaC3, 0.3);
        play(0, 0.2);
        play(notaC3, 0.3);
        play(0, 0.2);
        play(notaG3, 0.3);
        play(0, 0.2);
        play(notaG3, 0.3);
        play(0, 0.2);
        play(notaA3, 0.3);
        play(0, 0.2);
        play(notaA3, 0.3);
        play(0, 0.2);
        play(notaG3, 0.3);
        play(notaG3, 0.3);
        play(0, 0.2);

        update_button_state(S1ORS2, &fechado, &memoria);
        if (fechado == memoria) continue;
        else if (fechado) {
            if (button_pressed(S1)) {
                TB0CTL  ^= TBSSEL__SMCLK;
            }
            if (button_pressed(S2)) {
                // proxima musica
            }
        }
        //debounce(5000);
    }

    return 0;
}


#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_ISR()     //EXCLUSIVO CCR0
{
    switch (TB0IV) {
        case 0x2:              // Do canal 1
            //P1OUT ^= BIT1;
            break;
        case 0xE:              // Overflow
            P1OUT ^= BIT1;
            break;
        default:
            break;
    }
}

#pragma vector = TIMER1_B1_VECTOR
__interrupt void TB1_ISR()     //EXCLUSIVO CCR0
{
    switch (TB1IV) {
        case 0x2:              // Do canal 1
            break;
        case 0xE:              // Overflow
            durationFG = 1;
            TB0CTL &= ~TBSSEL__SMCLK;
            TB1CTL &= ~TBSSEL__ACLK;
            break;
        default:
            break;
    }
}

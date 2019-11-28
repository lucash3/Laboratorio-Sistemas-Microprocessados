#include <msp430.h> 
//#include "../biblioteca/LCD_base.c"
#include "../biblioteca/gpio.c"
#include "../biblioteca/clock.c"
#include "../biblioteca/buzzer.c"

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    UNLOCK_MASTER_PIN;

    config_all_buttons();
    config_all_leds();

    _Bool fechado = false;          // Aberto por padrão
    _Bool memoria = fechado;

    TB0CTL  = TBSSEL__SMCLK |        // Configura ACLK @32768 Hz
                  MC__UP       |        // Modo de contagem UP
                  TBIE         |        // Habilita interrupção de overflow
                  TBCLR;                // Começa do zero

    TB0CCTL1 = CCIE;                // Habilita interrupção localmente Do canal 1 (permite ver a flag ccifg)

    int estrelinha[14];
    /*
    [notaC3, notaC3, notaG3, notaG3, notaA3, notaA3, notaG3,
     notaF3, notaF3, notaE3, notaE3, notaD3, notaD3, notaC3];
    */

    int i = 0;
    while (i < 14) {
        if (i < 2 || i == 13)
            estrelinha[i] = notaC3;
        else if (i < 4 || i == 6)
            estrelinha[i] = notaG3;
        else if (i < 6)
            estrelinha[i] = notaA3;
        else if (i < 9)
            estrelinha[i] = notaF3;
        else if (i < 11)
            estrelinha[i] = notaE3;
        else
            estrelinha[i] = notaD3;
        i++;
    }
    i = 0;

    int freq = 656;
    TB0CCR0 = get_N(freq, FREQUENCE, MY_SMCLK);
    //TB0CCR1 = TB0CCR0 * 0.5;

    P1DIR |= BIT1;              // PXDIR = 1 => saida
    P1REN &= ~BIT1;
    P1OUT &= ~BIT1;

    __enable_interrupt();           // Seta GIE no registro SR

    while(1)
    {
        update_button_state(S1ORS2, &fechado, &memoria);
        if (fechado == memoria) continue;
            else if (fechado) {
                if (button_pressed(S1)) {
                    TB0CTL  ^= TBSSEL__SMCLK;
                }
                if (button_pressed(S2)) {
                    if (i == 14) i = 0;
                    freq = estrelinha[i++];
                    TB0CCR0 = get_N(freq, FREQUENCE, MY_SMCLK);
                }
            }
            debounce(5000);
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

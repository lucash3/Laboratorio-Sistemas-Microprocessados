#include <msp430.h> 
#include "../biblioteca/gpio.c"
#include "../biblioteca/clock.c"
#include "../biblioteca/buzzer.c"
#include "../biblioteca/songs.c"
#include "../biblioteca/I2C/lcd.c"


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    UNLOCK_MASTER_PIN;              // Destrava pinos digitais

    config_all_buttons();
    config_all_leds();

    i2cInit(1, 0);
    lcdInit();

    /*
     * Configurações para uso da função de botão da biblioteca gpio
     * fechado é o estado atual do botão
     * memória é o estado anterior
     *
     */

    _Bool fechado = false;
    _Bool memoria = fechado;

    /*
     * TB0 usado para gerar a PWM que gera a nota musical
     * TB1 usado para tocar a nota por um periodo de tempo determinado
     * */

    TB0CTL  = TBSSEL__SMCLK |
              MC__UP        |
              TBIE          |
              TBCLR;

    TB1CTL  = TBSSEL__ACLK |
              MC__UP       |
              TBIE         |
              TBCLR;

    TB0CCTL1 = CCIE;
    TB1CCTL1 = CCIE;

    // Saída para o buzzer

    P1DIR |= BIT1;
    P1REN &= ~BIT1;
    P1OUT &= ~BIT1;

    // Saída para o LED externo

    P5DIR |= BIT4;
    P5REN &= ~BIT4;
    P5OUT &= ~BIT4;

    __enable_interrupt();           // Seta GIE no registro SR

    // Índice de música na lista

    int musica = 0;

    while(1)
    {

        lcdClear();
        switch(musica) {
        case 0:
            lcdPrint("Jingle Bells\n");
            lcdPrint("200 bpm");
            jingleBell();
            break;
        case 1:
            lcdPrint("Happy Birthday\n");
            lcdPrint("80 bpm");
            happyBirthday();
            break;
        case 2:
            lcdPrint("Little Star\n");
            lcdPrint("120 bpm");
            littleStar();
            break;
        }

        // Checagem do botão pressionado
        update_button_state(S1ORS2, &fechado, &memoria);
        if (fechado == memoria) continue;
        else if (fechado) {
            if (button_pressed(S1)) {
                if (musica > 0) musica--;
            }
            if (button_pressed(S2)) {
                if (musica < 2) musica++;
            }
        }
        debounce(5000);
    }

    return 0;
}


/*
 * Interrupção usada para gerar a PWM da nota musical
 * */

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_ISR()
{
    switch (TB0IV) {
        case 0xE:              // Overflow
            P1OUT ^= BIT1;
            break;
        default:
            break;
    }
}

/*
 * Interrupção para medir o tempo de duração da nota e interromper
 * quando necessário
 * */

#pragma vector = TIMER1_B1_VECTOR
__interrupt void TB1_ISR()
{
    switch (TB1IV) {
        case 0xE:              // Overflow
            durationFG = 1;
            TB0CTL &= ~TBSSEL__SMCLK;
            TB1CTL &= ~TBSSEL__ACLK;
            break;
        default:
            break;
    }
}

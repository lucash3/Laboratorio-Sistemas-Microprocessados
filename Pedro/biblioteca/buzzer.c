
/*
*   Definições de frequência da PWM da nota
*   Frequências adaptadas para o buzzer
*/

#define notaC3 260
#define notaC3s 277
#define notaD3 292
#define notaD3s 310
#define notaE3 328
#define notaF3 348
#define notaF3s 366
#define notaG3 390
#define notaG3s 413
#define notaA3 436
#define notaA3s 464
#define notaB3 490
/*
#define notaC4 516
#define notaC4s 553
#define notaD4 584
#define notaD4s 621
#define notaE4 656
#define notaF4
#define notaF4s
#define notaG4
#define notaG4s
#define notaA4
#define notaA4s
#define notaB4
*/

/*
*   Dado a duração e a nota, configura as interrupções dos timers
*   para executar as notas de acordo
*/

volatile int durationFG = 0;

// Nota em Hz, duração em segundos
void play(int note, float duration) {

    TB0CCR0 = get_N(note, FREQUENCE, MY_SMCLK);
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

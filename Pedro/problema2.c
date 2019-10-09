#include <msp430.h>

/*
    CONEXOES
    FR2355  <--->   TCS3200 (sensor)
    P6.0    ---->   S0
    P6.1    ---->   S1
    P6.2    ---->   S2
    P6.3    ---->   S3
    P1.6    <----   OUT

    No sensor TCS3200, os pinos sao:
    Pinos   ---->   Funcao                            (00, 01, 10, 11)
    S0, S1  ---->   Seletores de escala de frequencia (0, 2, 20 ou 100%)
    S2, S3  ---->   Seletores de leitura de cor (Verm. Azul, Branca, Verde)
    OUT     ---->   Saida do sinal de frequencia gerado
*/

// *** AINDA PRECISA ADICIONAR CONFIGURACOES DE PINO ***

void main() {
    unsigned char counter = 0;
    int n, x, y, diff;
    int R, G, B;

    P6OUT |= BIT0;      // seleciona escala do sinal
    P6OUT &= ~BIT1;     // em 20% da frequencia original
                        // S0 = 1; S1 = 0

    // MAIN LOOP ---------------------------------------------------------------
    while (1) {

        // SELECAO DO MODO DE LEITURA ------------------------------------------
        // com base no counter
        switch (counter) {
            case 0:
                P6OUT &= ~(BIT2|BIT3);  // seleciona p/ luz vermelha
                break;                  // S2 = 0; S3 = 0

            case 1:
                P6OUT |= BIT3;          // seleciona p/ luz azul
                P6OUT &= ~BIT2;         // S2 = 0; S3 = 1
                break;

            case 3:
                P6OUT |= (BIT2|BIT3);   // seleciona p/ luz verde
                break;                  // S2 = 1; S3 = 1

            default: break;
        }

        // LEITURA DO SINAL RECEBIDO -------------------------------------------
        while (!(TB0CCTL1 & CCIFG));    // espera flanco do sinal recebido
        TB0CCTL1 &= ~CCIFG;
        x = TB0CCR1;                    // salva contagem de tempo do sinal
                                        // repete para variavel y
        while(!(TB0CCTL1 & CCIFG));
        TB0CCTL1 &= ~CCIFG;
        y = TB0CCR1;

        diff = y - x;                   // diff = periodo do sinal recebido
                                        // periodo indica intensidade luminosa
                                        // maior -> luz mais intensa
                                        // menor -> luz mais fraca


        // PASSAGEM DE CONTADOR PARA OUTRAS LEITURAS --------------------------
        // e atribuicao de valor lido p/ variaveis de cor
        switch (counter) {
            case 0:
                R = diff;               // intensidade vermelha salva
                counter++;              // counter: 0 -> 1
                break;                  // prox leitura: azul

            case 1:
                B = diff;               // intensidade azul salva
                counter++;              // counter: 1 -> 3
                counter++;              // prox leitura: verde
                break;

            case 3:
                G = diff;               // intensidade verde salva
                counter = 0;            // counter: 3 -> 0
                                        // prox leitura: vermelho (reset)
                flag = 1;               // seta flag p/ fazer comparacao
                break;                  // das intensidades luminosas
        }


        // INTERPRETACAO DAS LEITURAS ------------------------------------------
        // flag so e setada quando as leituras
        // R, G e B ja foram executadas
        if (flag == 1) {
            TB0CTL = MC_1     |         // reseta contador
                     TASSEL.1 |
                     TACLR;

            TB0CCTL1 = CM_1 |
                       SCS  |
                       CAP;

            // algoritmo de comparacao de maior numero
            maior = vermelho;
            n = 0;
            if (maior < verde) {
                maior = verde;
                n = 1;
            }
            if (maior < azul) {
                maior = azul;
                n = 2;
            }

            // FEEDBACK DA LEITURA ---------------------------------------------
            // LEDs:
            // Vermelho:    P1.0
            // Verde:       P6.6

            // Sinal dominante  ----    LEDs
            // Vermelho         ----    R = 1; G = 0
            // Verde            ----    R = 0; G = 1
            // Azul             ----    R = 1; G = 1

            switch (n) {
                case 0:                 // caso vermelho dominante
                    P1OUT |= BIT0;      // R = 1
                    P6OUT &= ~BIT6;     // G = 0
                    break;

                case 1:                 // caso verde dominante
                    P1OUT &= ~BIT0;     // R = 0
                    P6OUT |= BIT6;      // G = 1
                    break;

                case 2:                 // caso azul dominante
                    P1OUT |= BIT0;      // R = 1
                    P6OUT |= BIT6;      // G = 1
            }

            flag = 0;
        }
    }
}

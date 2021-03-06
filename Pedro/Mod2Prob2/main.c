// ALUNOS:
// Pedro Victor Rodrigues de Carvalho   17/0113043
// Victor Eduardo Fernandes Castor      17/0115127

#include <msp430.h>
#include <stdint.h>

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
    S2, S3  ---->   Seletores de leitura de cor (Verm., Azul, Branca, Verde)
    OUT     ---->   Saida do sinal de frequencia gerado
*/

void main() {
    unsigned char seletor = 0, flag = 0;
    unsigned int select_cor, medida1, medida2, diff, maior;
    unsigned int R, G, B;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);     // destrava pinos digitais

    // CONFIGURACOES DE PINO ---------------------------------------------------
    TB0CTL = MC__CONTINUOUS |         // configuracao de timer
             TBSSEL__ACLK;

    TB0CCTL1 = CM_1 |
               SCS  |
               CAP;

    P6DIR |= (BIT0|BIT1|BIT2|BIT3);     // habilita saída nos pinos
    P6REN &= ~(BIT0|BIT1|BIT2|BIT3);    // desabilita resistor

    P1DIR &= ~BIT6;     // habilita entrada no pino
    P1SEL1 |= BIT6;     // habilita funcionalidade dedicada

    P1DIR |= BIT0;      // habilita saida no pino (LED vermelho)
    P1OUT &= ~BIT0;     // LED desligado
    P6DIR |= BIT6;      // habilita saida no pino (LED verde)
    P6OUT &= ~BIT6;     // LED desligado

    // INICIO ------------------------------------------------------------------
    P6OUT |= BIT0;      // seleciona escala do sinal
    P6OUT &= ~BIT1;     // em 20% da frequencia original
                        // S0 = 1; S1 = 0

    // MAIN LOOP ---------------------------------------------------------------
    while (1) {

        // SELECAO DO MODO DE LEITURA ------------------------------------------
        // com base no seletor
        switch (seletor) {
            case 0:
                P6OUT &= ~(BIT2|BIT3);  // seleciona p/ luz azul
                break;                  // S2 = 0; S3 = 0

            case 1:
                P6OUT |= (BIT2|BIT3);   // seleciona p/ luz vermelho
                break;                  // S2 = 1; S3 = 1

            case 2:
                P6OUT |= BIT3;          // seleciona p/ luz verde
                P6OUT &= ~BIT2;         // S2 = 0; S3 = 1
                break;

            default: break;
        }

        // LEITURA DO SINAL RECEBIDO -------------------------------------------
        while (!(TB0CCTL1 & CCIFG));    // espera flanco do sinal recebido
        TB0CCTL1 &= ~CCIFG;
        medida1 = TB0CCR1;              // salva contagem de tempo do sinal
                                        // repete medicao
        while(!(TB0CCTL1 & CCIFG));
        TB0CCTL1 &= ~CCIFG;
        medida2 = TB0CCR1;

        diff = medida2 - medida1;       // diff = periodo do sinal recebido
                                        // periodo indica intensidade luminosa
                                        // maior -> luz mais intensa
                                        // menor -> luz mais fraca

        // PASSAGEM DE CONTADOR PARA OUTRAS LEITURAS --------------------------
        // e atribuicao de valor lido p/ variaveis de cor
        switch (seletor) {
            case 0:
                B = diff;               // intensidade azul salva
                seletor++;              // seletor: 0 -> 1
                break;                  // prox leitura: vermelho

            case 1:
                R = diff;               // intensidade vermelho salva
                seletor++;              // seletor: 1 -> 2
                break;                  // prox leitura: verde

            case 2:
                G = diff;               // intensidade verde salva
                seletor = 0;            // seletor: 2 -> 0
                                        // prox leitura: azul (reset)
                flag = 1;               // seta flag p/ fazer comparacao
                break;                  // das intensidades luminosas
        }

        // INTERPRETACAO DAS LEITURAS ------------------------------------------
        // flag so e setada quando as leituras
        // R, G e B ja foram executadas
        if (flag == 1) {
            // algoritmo de comparacao de maior numero
            maior = R;
            select_cor = 0;
            if (maior < G) {
                maior = G;
                select_cor = 1;
            }
            if (maior < B) {
                maior = B;
                select_cor = 2;
            }

            // FEEDBACK DA LEITURA ---------------------------------------------
            // LEDs:
            // Vermelho:    P1.0
            // Verde:       P6.6

            // Sinal dominante  ----    LEDs
            // Vermelho         ----    R = 1; G = 0
            // Verde            ----    R = 0; G = 1
            // Azul             ----    R = 1; G = 1

            switch (select_cor) {
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
                    break;
            }

            flag = 0;
        }
    }
}

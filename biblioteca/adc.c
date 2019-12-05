#include <msp430.h>
#include <stdint.h>

/*
*	pin é o bit do pino
*/

uint16_t adcRead(uint8_t pin)
{
	// Atualizando registros de controle ADC
	ADCCTL0 = ADCSHT_6 | ADCON;					// Amostra de 128 ciclos (Cálculo necessário), e ligando ADC (NES. ADCENC = 0)
	ADCCTL1 = ADCSHS_0 | ADCCONSEQ_0 | ADCSHP;	// Inicia amostragem, Modo de sequência 1 Canal - 1 Conversão, 
												// seleção de modo de pulso(Fonte selecionada: Sampling Timer(permite divisões))(NES. ADCENC = 0)
	ADCCTL2 = ADCRES_2;							// Resolução do sinal de saída -> 12 bits
	
	// Registro de controle da memória
	ADCMCTL0 = pin;								// Seleciona o canal bit 'pin' do pino 1			
	
	// Funcionalidade dedicada do pino 1 bit 'pin'
	P1SEL0 |= 1 << pin;
	P1SEL1 |= 1 << pin;


	ADCCTL0 |= ADCENC;							// Habilita conversão
	ADCCTL0 &= ~ADCSC;							// Gera flanco de subida
	ADCCTL0 |= ADCSC;							//

	while(!(ADCIFG & ADCIFG0));					// ADCIFG0 (pertencente ao ADCIFG) é setado quando a conversão é completa
	return ADCMEM0;								// Retorna resultado da conversão
}

/*
*	Usando clock externo
*/

/*
void adcConfig()
{
	ADCCTL0 = ADCCON;							// Liga ADC
	ADCCTL1 = ADCSHS_2 | ADCCONSEQ_2;			// Amostragem com trigger 1 de timer, Repete mesmo  sinal
	ADCCTL2 = ADCRES_1;							// Resolução de 10 bits

	ADCMCTL0 = 6;

	P1SEL0 |= BIT6;
	P1SEL1 |= BIT6;

	ADCCTL0 |= ADCENC;							// Havilita conversão
	ADCIE = ADCIE0;								// Flag de interrupção

	TB1CTL = MC__UP | 
			 TBCLR  | 
			 TBSSEL_SMCLK;
	TB1CCR0 = 50000 - 1;
	TB1CCR1 = 6900 - 1;
	TB1CCTL1 = OUTMODE_7; 
}
*/


void adcConfigMulti()
{
	ADCCTL0 = ADCON | ADCMSC;					// Liga ADC, Múltiplas amostras de conversões automáticas (Apenas para sequências ou modos repetidos) (1º trigger necessário)
	ADCCTL1 = ADCSHS_2 | ADCCONSEQ_3 | ADCSHP;	// Amostragem com trigger 1 de timer, Vários canais repetidos, seleção de modo de pulso(Fonte selecionada: Sampling Timer(permite divisões))
	ADCCTL2 = ADCRES_1;							// Resolução de 10 bits

	ADCMCTL0 = 1;								// Começa lendo no A1 e vai pra A0 e fica nesse ciclo

	P1SEL0 |= (BIT1 | BIT0); 					// A0 e A1
	P1SEL1 |= (BIT1 | BIT0);

	ADCCTL0 |= ADCENC;							// Havilita conversão
	ADCIE = ADCIE0;								// Flag de interrupção

	TB1CTL = MC__UP | 
			 TBCLR  | 
			 TBSSEL__ACLK;
	TB1CCR0 = 0x1000 - 1;							// 8 Hz
	TB1CCR1 = 0x800 - 1;							// 4 Hz
	TB1CCTL1 = OUTMOD_7; 						// set/reset	-> 8 ciclos por segundo
}

#include <msp430.h>
#include <stdint.h>

/*
*	Inicialização da interface I2c.
*	P1.2 e P1.3
*/
void i2cInit() 
{
	// UCBCTLW0 -> Registro de Configuração
	UCB0CTLW0 = UCSSEL__SMCLK	|
				UCMODE_3		| 	// Modo I2C
				UCSWRST			|
				UCMST			|	// 1 -> Mestre
	UCB0BRW = 10; // Bold Rate Divider -> Dividir o clock de entrada.
	// 1M / 10 = 100 kHz

	P1SEL0 |=  (BIT2 | BIT3);	// Selecionar funcionalidade dedicada
	P1SEL1 &= ~(BIT2 | BIT3);	// de I2C do pino
	P1DIR  &= ~(BIT2 | BIT3);	// Habilita resistores de Pull-up
	P1REN  |=  (BIT2 | BIT3);
	P1OUT  |=  (BIT2 | BIT3);

	UCB0CTLW0 &= ~UCSWRST; // Só é necessário na configuração.
}

/*
*	Testar se o addr está ou não presente no barramento
*	Aguardar escravo responder com ACK (presente).
*/
uint8_t i2cTst(uint8_t addr) 
{
	UCB0IFG = 0; 							// Zera as flags, pois NACK não
											// é zerado no start (coloca em 
											// estado conhecido)

	UCB0I2CSA  = addr;						// Configura end. do escravo (Slave Addr)
	UCB0CTLW0 |= UCTR + UCTXSTT;			// Pede um start (Transmissão + Start)

	while(!(UCB0IFG & UCTXIFG));			// Aguarda UCTXIFG == 1, o reg. de transm.
											// está vazio e deve ser preenchido.
	UCB0TXBUF = 0x00;						// Escreve um byte dummy para
											// destravar o ciclo de ACK

	while( !(UCB0IFG & UCNACKIFG) &&		// Espera ou NACK
		   !(UCB0IFG & UCTXIFG) );			// ou TXIFG (== 1 quando ACK) (Flag de transm.)

		/*	Bug MSP430 FR2355
		*	UCTXSTT não vai para zero no momento de ACK ou NACK.
		*/

	UCB0CTLW0 |= UCTXSTP;					// Pede o stop e
	while(UCB0CTLW0 & UCTXSTP);				// aguarda ele ser enviado

	return (UCB0IFG & UCNACKIFG); 			// Retorna ACK (0) ou NACK (1)
}

/*
*	Coloca sequência de bytes no barramento por meio do pData
*	O número de bytes é dado por count.
*	addr é o endereço do escravo em comunicação.
*/
uint8_t i2cPut(uint8_t addr, uint8_t *pData, uint8_t count) 
{
	UCB0IFG = 0; 							// Zera as flags, pois NACK não
											// é zerado no start (coloca em 
											// estado conhecido)

	UCB0I2CSA  = addr;						// Configura end. do escravo (Slave Addr)
	UCB0CTLW0 |= UCTR + UCTXSTT;			// Pede um start (Transmissão + Start)

	while(!(UCB0IFG & UCTXIFG));			// Aguarda UCTXIFG == 1, o reg. de transm.
											// está vazio e deve ser preenchido.
											
	UCB0TXBUF = *pData++;					// Escreve o primeiro byte (reseta UCTXIFG)
	count--;								// para destravar o ciclo de ACK

	while( !(UCB0IFG & UCNACKIFG) &&		// Espera ou NACK
		   !(UCB0IFG & UCTXIFG) );			// ou TXIFG (== 1 quando ACK) (Flag de transm.)

		/*	Bug MSP430 FR2355
		*	UCTXSTT não vai para zero no momento de ACK ou NACK.
		*/

	if (UCB0IFG & UCNACKIFG)
	{											
		UCB0CTLW0 |= UCTXSTP;				// Se for NACK pede stop
		while(UCB0CTLW0 & UCTXSTP);			// aguarda ele ser enviado
		return 1; 							// Retorna erro
	}

	while(count--)							// Continua com o envio de bytes
	{
		while(!(UCB0IFG & UCTXIFG));
		UCB0TXBUF = *pData++;
	}

	UCB0CTLW0 |= UCTXSTP;					//Stop, pois todos os bytes foram enviados
	while(UCB0CTLW0 & UCTXSTP);
	return 0;								// Sucesso

}

/*
*	Recebe dados do barramento.
*/
uint8_t i2cGet(uint8_t addr, uint8_t *pData, uint8_t count) 
{
	UCB0IFG = 0; 							// Zera as flags, pois NACK não
											// é zerado no start (coloca em 
											// estado conhecido)

	UCB0I2CSA  = addr;						// Configura end. do escravo
	UCB0CTLW0 &= ~UCTR;						// Modo receptor
	UCB0CTLW0 |= UCTXSTT;					// Pede start como receptor

	while(UCB0IFG & UCTXSTT);				// Espera o ciclo de ACK
											// (não funciona na FR2355)
	if (UCB0IFG & UCNACKIFG)
	{											
		UCB0CTLW0 |= UCTXSTP;				// Se for NACK pede stop
		while(UCB0CTLW0 & UCTXSTP);			// aguarda ele ser enviado
		return 1; 							// Retorna erro
	}

	while(--count)
	{
		while(!(UCB0IFG & UCRXIFG));		// Realiza a recepção de n-1 bytes
		*pData++ = UCB0RXBUF;
	}

	UCB0CTLW0 |= UCTXSTP;					// Antes do último byte,
											// requisita stop

	while(!(UCB0IFG & UCRXIFG));			// Faz leitura do último byte
	*pData++ = UCB0RXBUF;

	while(UCB0CTLW0 & UCTXSTP);				// Aguarda stop

	return 0;								// sucesso
}

uint8_t i2cSendByte(uint8_t addr, uint8_t byte)
{
	return i2cPut(addr, &byte, 1);
}

uint8_t i2cGetByte(uint8_t addr)
{
	uint8_t byte;
	i2cPut(addr, &byte, 1);
	return byte
}
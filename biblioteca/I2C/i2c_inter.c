#include <msp430.h>
#include <stdint.h>

/*
*	Usando interrupções
*/

struct {
	uint8_t status;
	uint8_t data;
} UCB0;

/*
*	Inicialização da interface I2c.
*	P1.2 e P1.3 (SDA e SCL)
*/
void i2cInit(uint8_t isMaster, uint8_t myAddr) 
{
	UCB0CTLW0 = UCMODE_3 | UCSWRST;			// Modo: I2C
	UCB0CTLW1 = UCASTP_2;					// com stop automático
	UCB0TBCNT = 1;							// para recepção de 1 byte

	if (isMaster)							// Se for mestre
	{
		UCB0CTLW0 |= UCSSEL__SMCLK |		// Configura como Mestre
					 UCMST;
		UCB0BRW = 50;						// SCL @ 100kHz
	}
	else									// Se for escravo
	{
		UCB0I2COA0 = UCOAEN | myAddr;		// Configura o endereço (own addr)
	}

	P1SEL0 |=  (BIT2 | BIT3);				// Seleciona funcionalidade dedicada
	P1SEL1 &= ~(BIT2 | BIT3);				// SDA e SCL -> P1.2 e P2.3
	P1DIR  &= ~(BIT2 | BIT3);				// Habilita resistor de Pull-up
	P1REN  |=  (BIT2 | BIT3);
	P1OUT  |=  (BIT2 | BIT3);

	UCB0CTLW0 &= ~UCSWRST; 					// Só é necessário na configuração.
											// Libera a interface para operar
											// zerando o bit de reset

											// Interrupções:
	UCB0IE = UCRXIE	 |						// RX
			 UCTXIE  |						// TX
			 UCSTPIE |						// Stop
			 UCNACKIE;						// NACK
}

/*
*	Testar se o addr está ou não presente no barramento
*	Aguardar escravo responder com ACK (presente).
*/
uint8_t i2cTst(uint8_t addr) 
{
	UCB0.status = 0; 						// Considera que a transmissão
											// será bem sucedida

	UCB0I2CSA  = addr;						// Configura end. do escravo (Slave Addr)
	UCB0CTLW0 |= UCTR + UCTXSTT + UCTXSTP;	// Pede start e stop simultâneos (não eniva bytes)
											// ativando também a transmissão

	__low_power_mode_0();					// Aguarda o final da comunicação
	return UCB0.status;						// para retornar ACK (0) ou NACK (1)
}

/*
*	Coloca sequência de bytes no barramento por meio do pData
*	O número de bytes é dado por count.
*	addr é o endereço do escravo em comunicação.
*/
uint8_t i2cPut(uint8_t addr, uint8_t data) 
{
	UCB0.status = 0;						// Considera que a transmissão
											// será bem sucedida

	UCB0I2CSA = addr;						// Configura end. do escravo (Slave Addr)
	UCB0.data = data;						// e copia o byte para a interface

	UCB0CTLW0 |= UCTR + UCTXSTT;			// Pede início da comunicação

	__low_power_mode_0();					// Aguarda o final da comunicação
	return UCB0.status;						// para retornar ACK (0) ou NACK (1);
}

/*
*	Recebe dados do barramento.
*/
uint8_t i2cGet(uint8_t addr, uint8_t *pData) 
{
	UCB0.status = 0;						// Considera que a transmissão
											// será bem sucedida

	UCB0I2CSA = addr;						// Configura end. do escravo

	UCB0CTLW0 &= ~UCTR;						// Pede start como receptor
	UCB0CTLW0 |= UCTXSTT;

	__low_power_mode_0();

	*pData = UCB0.data;						// Grava o byte no ponteiro do usuário

	return UCB0.status;						// para retornar ACK (0) ou NACK (1);
}

#pragma vector = USCI_B0_VECTOR
__interrupt void I2C_ISR()
{
	switch (UCB0IV)
	{
		case UCIV__UCNACKIFG:				// Se recebeu um NACK		
			UCB0CTLW0 |= UCTXSTP;			// Pede um stop
			UCB0.status = 1;				// E avise o usuário do nack
			break;

		case UCIV__UCSTPIFG:				// Se enviou o stop
			__low_power_mode_off_on_exit();	// volta para o programa principal
			break;							// saindo do modo de baixo consumo

		case UCIV__UCRXIFG0:				// Se recebeu algo no buffer
			UCB0.data = UCB0RXBUF;			// do usuário
			break;

		case UCIV__UCTXIFG0:				// Se for transmitir algo, pegue
			UCB0TXBUF = UCB0.data;			// o byte da posição indicada  pelo
			break;							// ponteiro do usuário

		default:
			break;
	}
}

/*

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
*/

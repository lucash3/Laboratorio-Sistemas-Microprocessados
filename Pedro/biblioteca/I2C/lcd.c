#include <stdint.h>
#include "i2c_inter.c"

uint8_t lcdAddr = 0x27;
const uint8_t lcdRS = 0x01;
const uint8_t lcdRW = 0x02;
const uint8_t lcdEN = 0x04;
const uint8_t lcdBL = 0x08;

void lcdWriteNibble(uint8_t nibble, uint8_t isChar)
{
	nibble <<= 4;							// Ajusta a posição do nibble
	nibble |= lcdBL | isChar;				// Backlight on
											// isChar para caso não seja instrução

	i2cPut(lcdAddr, nibble |  0  );			// Gera pulso
	i2cPut(lcdAddr, nibble |lcdEN);			// em Enable
	i2cPut(lcdAddr, nibble |  0  );
}

void lcdWriteByte(uint8_t byte, uint8_t isChar)
{
	lcdWriteNibble(byte >> 4, isChar);		// + significativa
	lcdWriteNibble(byte 	, isChar);		// - significativa
}

uint8_t lcdReadNibble(int pos_cursor)
{
	uint8_t nibble = 0;
	uint8_t dado;

	lcdWriteByte(0x02, 0);					// Cursor 1ª pos

	while(--pos_cursor)
	{
		lcdWriteByte(0x14, 0);				// Desloca cursor para direita 
	}

	nibble |= lcdBL | lcdRW | lcdEN;        // Configura leitura // Acho que não deveria ter enable aqui

    i2cPut(lcdAddr, nibble |  0  );         // Gera pulso
    i2cPut(lcdAddr, nibble |lcdEN);         // em Enable
    i2cGet(lcdAddr, &dado);
    i2cPut(lcdAddr, nibble |  0  );

	return dado;
}

uint8_t lcdReadByte(int pos_cursor)
{
    uint8_t nibble1 = lcdReadNibble(pos_cursor) & 0xF0;
	uint8_t nibble2 = lcdReadNibble(pos_cursor);
	uint8_t byte;
	nibble2 >>= 4;

	byte = nibble1 | nibble2;

	return byte;
} 

/*
*	Precisa garantir que vai entrar no modo 4 bits
*/

/*
void lcdBusy()
{
	while(lcdReadByte() & 0x80);			// Se o lcd está ou não ocupado
}
*/

void lcdInit()
{
	if(i2cTst(lcdAddr))						// Verifica se o LCD está no end.
		lcdAddr = 0x3F;						// 0x27 ou 0x3F

	lcdWriteNibble(0x3, 0);					// Entra em modo 8-bits
	lcdWriteNibble(0x3, 0);
	lcdWriteNibble(0x3, 0);
	lcdWriteNibble(0x2, 0);					// Entra em modo 4-bits

	lcdWriteByte(0x28, 0);					// Configura: Duas linhas
	lcdWriteByte(0x0C, 0);					// Display ON, Cursor ON
	lcdWriteByte(0x06, 0);					// Modo de incremento

	lcdWriteByte(0x01, 0);					// Limpa a tela (cursor em 1ª pos)

	//lcdBusy();								// Espera ficar disponível
}

void lcdClear()
{
	lcdWriteByte(0x01, 0);					// Limpa a tela (cursor em 1ª pos)	
}

void lcdResetaCursor()
{
	lcdWriteByte(0x02, 0);
}

void lcdPrint(char *str)
{
	static uint8_t line = 0x80;

	while(*str)								// Enquanto o byte não for 0
	{
		if(*str == '\n')
		{
 			line ^= 0x40; 
			lcdWriteByte(line, 0);
			str++;
		}
		else
			lcdWriteByte(*str++, 1);

	}
}

void lcdPrintaHexa(int numeroIn){

    int k = 0, j = 4, valor;

    char numeroHexa[4];

    while (k < 4) {
    	numeroHexa[k] = 48;
		k++;
    }

    k = 0;

    while(numeroIn != 0){
         valor = numeroIn % 16;

      // Conversão inteiro para caractere
      if(valor < 10)
           valor = valor + 48;
      else
         valor = valor + 55;

      numeroHexa[k++] = valor;
      numeroIn = numeroIn / 16;
    }

    while(j) {
      lcdWriteByte(numeroHexa[j-1], 1);
	  j--;
    }
}

/*
void lcdPrintaHexa(int quotient){

    int k = 0, j = 4, temp;

    char hexadecimalNumber[4];

    hexadecimalNumber[0] = 32;
    hexadecimalNumber[1] = 32;
    hexadecimalNumber[2] = 32;
    hexadecimalNumber[3] = 32;

    while(quotient != 0){
         temp = quotient % 16;

      // Conversão inteiro para caractere
      if(temp < 10)
           temp = temp + 48;
      else
         temp = temp + 55;

      hexadecimalNumber[k++] = temp;
      quotient = quotient / 16;
    }

    while(j--) {
      lcdWriteByte(hexadecimalNumber[j-1], 1);
    }
}
*/

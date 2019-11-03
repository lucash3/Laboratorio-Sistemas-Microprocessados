#include "gpio.c"

int lcdInit (char addr) {
    reconPCF(addr);             // PCF em estado conhecido

    // Preparar I2C para operar
    UCB0I2CSA = addr;                   // Endereço Escravo
    UCB0CTL1 |= UCTR | UXTSTT;          // tranf mestre; START
    while (!(UCB0IFG & UCTXIFG));       // Esperar TXIFG=1

    UCB0TXBUF = 0;                      // Saída PCF = 0;
    while (!(UCB0IFG & UCNACKIFG) &&    // Espera NACK ou ACK
           !(UCB0IFG & UCTXIFG));

    if (UCB0IFG & UCNACKIFG)            // retorna 1 caso NACK
        return UCB0IFG & UCNACKIFG;

    // Inicializacao
    giveNibble(0);        // RS = 0; RW = 0; BL = 1
    debounce(20000);
    giveNibble(3);
    debounce(20000);
    giveNibble(3);
    debounce(20000);
    giveNibble(3);
    debounce(20000);
    giveNibble(2);

    // Modo 4 bits
    giveByte(0x28);
    giveByte(0x08);
    giveByte(0x01);
    giveByte(0x06);
    giveByte(0x0F);

    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0CTL1 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    debounce(50);

}

void reconPCF (char addr) {
    UCB0I2CSA = addr;
    int cont = 6;

    while (cont--) {
        UCB0CTL1 |= UCTR | UCTXSTT;     // transf mestre; START
        while (!(UCB0IFG & UCTXIFG));   // Espera TXIFG = 1
        UCB0CTL1 |= UCTXSTP;            // STOP
        debounce(200);
        if (!(UCB0CTL1 & UCTXSTP))      // termina caso STOP
            break;
    }
}

/*  -----------------------------------------------------
    Funcoes "give" ---> funcionam somente com RS = RW = 0
    -----------------------------------------------------
*/

void giveNibble(char data){
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data << 4) & 0xF0) | BIT_BL;      //PCF7:4 = dado;
    debounce(50);
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data << 4) & 0xF0) | BIT_BL | BIT_E;  //E=1
    debounce(50);
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data << 4) & 0xF0) | BIT_BL;      //E=0;
}

void giveByte(char data){
    // primeiro nibble
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data << 4) & 0xF0) | BIT_BL;      //PCF7:4 = dado;
    debounce(50);
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data << 4) & 0xF0) | BIT_BL | BIT_E;  //E=1
    debounce(50);
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data << 4) & 0xF0) | BIT_BL;      //E=0;

    // segundo nibble
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data) & 0xF0) | BIT_BL;      //PCF7:4 = dado;
    debounce(50);
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data) & 0xF0) | BIT_BL | BIT_E;  //E=1
    debounce(50);
    while (!(UCB0IFG & UCTXIFG));                   //Esperar TXIFG=1
    UCB0TXBUF = ((data) & 0xF0) | BIT_BL;      //E=0;
}

void lcdWriteNibble (uint8_t nibble, unsigned char rs) {
    i2csend(nibble << 4 | 0x08 | rs);
    i2csend(nibble << 4 | 0x0C | rs);
    i2csend(nibble << 4 | 0x08 | rs);
}

void lcdWriteByte (char data, char addr) {
    UCB0IFG = 0;

    UCB0I2CSA = addr;               //Endereço Escravo
    UCB0CTL1 |= UCTR | UCTXSTT;     // transf mestre; START

    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0TXBUF = data;                              //Escrever dado

    while ( !(UCB0IFG & UCNACKIFG) && !(UCB0IFG & UCTXIFG) );

    UCB0CTL1 |= UCTXSTP;    					//Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP

    return UCB0IFG & UCNACKIFG;
}

void lcdWriteStr (char* str) {

}

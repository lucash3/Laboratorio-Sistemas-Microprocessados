void i2cInit () {
    P1SEL0 |= BIT2 | BIT3;                  // funcao dedicada dos pinos
    P1SEL1 &= ~(BIT2 | BIT3);               // p/ SDA e SCL do I2C
    P1REN |= BIT2 | BIT3;                   // resistores de
    P1OUT |= BIT2 | BIT3;                   // pull-up habilitados
    UCB0CTLW0 |= UCSWRST;                   // reseta eUSCI_B
    UCB0CTLW0 |= UCMODE_3 | UCMST;          // modo Mestre I2C, SMCLK
    UCB0BRW = BR100K;                       // baudrate = SMCLK / 10
    UCB0CTLW0 &=~ UCSWRST;                  // limpa registro de reset
    //UCB0IE |= UCTXIE0 | UCNACKIE;         // interrupt enable de transmit e NACK
}

void i2cSend (char dstAddr, *dataPtr, nBytes) {
    
}

void i2cGet (dstAddr, *dataPtr, nBytes) {

}

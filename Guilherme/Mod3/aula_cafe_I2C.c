void i2cInit(){

  UCB0CTLW0 = (UCSSEL__SMCLK | UCMODE_3 | UCSWRST | UCMST);
  UCB0BRW = 10;

  P1SEL0 |= (BIT2 | BIT3);
  P1SEL1 &= ~(BIT2 | BIT3);
  P1DIR &= ~(BIT2 | BIT3);
  P1REN |= (BIT2 | BIT3);
  P1OUT |= (BIT2 | BIT3);

  UCB0CTLW0 &= ~UCSWRST;

}

uint8_t i2cTst(uint8_t addr){

  UCB0IFG = 0;

  UCB0I2CSA = addr;
  UCB0CTLW0 |= UCTR + UCTXSST;

  while( !(UCB0IFG & UCNACKIFG) && !(UCB0IFG & UCTXIFG));

  UCB0CTLW0 |= UCTXSTP;

  return (UCB0IFG & UCNACKIFG);

}

uint8_t i2cPut(uint8_t addr, uint8_t *data, uint8_t count){

  UCB0IFG = 0;                                              // zera pq nack não é zerado no start

  UCB0I2CSA = addr;                                         // end escravo
  UCB0CTLW0 |= UCTR + UCTXSTT;                              // pede start

  while(!(UCB0IFG & UCNACKIFG) && !(UCB0IFG & UCTXIFG));    // Espera ou NACK ou TXIFG

  if(UCB0IFG & UCNACKIFG){                                  // se há um NACK
    UCB0CTLW0 |= UCTXIFG;                                   // pede stop
    while(UCB0CTLW0 & UCTXSTP);                             // aguarda envio
    return 1;                                               // retorna erro

  }

  while(count--){                                           // envia bytes restantes
    while(!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = *data++;
  }

  UCB0CTLW0 |= UCTXSTP;                                     // tendo enviado tudo, pedimos o stop e aguardamos o sucesso
  return 0;
}

uint8_t i2cGet(uint8_t addr, uint8_t *data, uint8_t count){

  UCB0IFG = 0;

  UCB0I2CSA = addr;                                         // end escravo
  UCB0CTLW0 &= ~UCTR;                                       // pede start como receptor
  UCB0CTLW0 |= UCTXSTT;

  while(UCB0IFG & UCNACKIFG);                               // espera ciclo de ACK (não funciona na FR2355)

  if(UCB0IFG & UCNACKIFG){                                  // se for um NACK
    UCB0CTLW0 |= UCTXSTP;                                   // pede stop e para
    while(UCB0CTLW0 & UCTXSTP);
    return 1;
  }

  while(--count){
    while(!(UCB0IFG & UCRXIFG));                            // recebe n-1 bytes
    *data++ = UCB0RXBUF;
  }

  UCB0CTLW0 |= UCTXSTP;                                     // antes do ultimo, stop

  while(!(UCB0IFG & UCRXIFG));
  *data++ = UCB0RXBUF;                                      // faz a leitura

  while(UCB0CTLW0 & UCTXSTP);                                // aguarda envio do stop

  return 0;

}

uint8_t i2cSendByte(uint8_t addr, uint8_t byte){

  return i2cPut(addr, &byte, 1);

}

uint8_t i2cGetByte(uint8_t addr){

  uint8_t byte;
  i2cGet(addr, &byte, 1);
  return byte;

}

#include <msp430.h>
// Básico para o LCD funcionar
// P3.0 = SDA e P3.1=SCL

#define BR100K 	10	//SCL operando em 100 kHz (SMCLK)
#define BR10K   21	//SCL operando em 50 kHz (SMCLK)
//#define BR10K   105	//SCL operando em 10 kHz (SMCLK)

#define BIT_BL	BIT3	//Back light
#define BIT_E 	BIT2	//Enable

// Dois possíveis endereços do PCF8574
#define PCF_ADR	0x27
//#define PCF_ADR	0x3F


// Protótipo das funções
void config_I2C(void);
void LCD_inic(void);
void LCD_aux(char dado);
void PCF_STT_STP(void);
int PCF_read(void);
int PCF_write(char dado);
void delay(long limite);

/*
void main(void){
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
	config_I2C();
	LCD_inic();
	//LCD pronto para usar
	...
}
*/


// Configurar Pinos I2C - UCSB0
// P3.0 = SDA e P3.1=SCL
void config_I2C(void){
	P1SEL0 |= BIT2 | BIT3;
	P1SEL1 &= ~(BIT2 | BIT3);
	P1REN |= BIT2 | BIT3;
	P1OUT |= BIT2 | BIT3;
	UCB0CTLW0 |= UCSWRST;                             // put eUSCI_B in reset state
    UCB0CTLW0 |= UCMODE_3 | UCMST;                    // I2C master mode, SMCLK
    UCB0BRW = BR100K;                                   // baudrate = SMCLK / 8
    UCB0CTLW0 &=~ UCSWRST;                            // clear reset register
    //UCB0IE |= UCTXIE0 | UCNACKIE;                     // transmit and NACK interrupt enable
}



// Incializar LCD modo 4 bits
void LCD_inic(void){

    PCF_STT_STP();      //Colocar PCF em estado conhecido

    // Preparar I2C para operar
    UCB0I2CSA = PCF_ADR;    //Endereço Escravo
    UCB0CTL1 |= UCTR    |   //Mestre TX
                UCTXSTT;    //Gerar START
    while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar TXIFG=1
    UCB0TXBUF = 0;                              //Saída PCF = 0;
    while ( (UCB0IFG & UCNACKIFG) == 0 && (UCB0IFG & UCTXIFG) == 0);
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
                while(1);

    // Começar inicialização
    LCD_aux(0);     //RS=RW=0, BL=1
    delay(20000);
    LCD_aux(3);     //3
    delay(10000);
    LCD_aux(3);     //3
    delay(10000);
    LCD_aux(3);     //3
    delay(10000);
    LCD_aux(2);     //2

    // Entrou em modo 4 bits
    LCD_aux(2);     LCD_aux(8);     //0x28
    LCD_aux(0);     LCD_aux(8);     //0x08
    LCD_aux(0);     LCD_aux(1);     //0x01
    LCD_aux(0);     LCD_aux(6);     //0x06
    LCD_aux(0);     LCD_aux(0xF);   //0x0F

    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0CTL1 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    delay(50);
}


// Auxiliar inicialização do LCD (RS=RW=0)
// *** Só serve para a inicialização ***
void LCD_aux(char dado){
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0xF0) | BIT_BL;          //PCF7:4 = dado;
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0xF0) | BIT_BL | BIT_E;  //E=1
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0xF0) | BIT_BL;          //E=0;
}


// Gerar START e STOP para colocar PCF em estado conhecido
void PCF_STT_STP(void){
    int x=0;
    UCB0I2CSA = PCF_ADR;   		//Endereço Escravo

    while (x<5){
        UCB0CTL1 |= UCTR    |   //Mestre TX
                    UCTXSTT;    //Gerar START
        while ( (UCB0IFG & UCTXIFG) == 0);	//Esperar TXIFG=1
        UCB0CTL1 |= UCTXSTP;    			//Gerar STOP
        delay(200);
        if ( (UCB0CTL1 & UCTXSTP) == 0)   break;   //Esperar STOP
        x++;
    }
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);   //I2C Travado (Desligar / Ligar)
}


// Ler a porta do PCF
int PCF_read(void){
    int dado;
    UCB0I2CSA = PCF_ADR;        		//Endereço Escravo
    UCB0CTL1 &= ~UCTR;     				//Mestre RX
    UCB0CTL1 |= UCTXSTT;   				//Gerar START
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT);
    UCB0CTL1 |= UCTXSTP;    			//Gerar STOP + NACK
    while ( (UCB0IFG & UCRXIFG) == 0);	//Esperar RX
    dado=UCB0RXBUF;
    delay(50);
    return dado;
}


// Escrever dado na porta
int PCF_write(char dado){
    UCB0IFG = 0;

    UCB0I2CSA = PCF_ADR;        //Endereço Escravo
    UCB0CTL1 |= UCTR    |       //Mestre TX
                UCTXSTT;        //Gerar START

    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0TXBUF = dado;                              //Escrever dado

	while ( !(UCB0IFG & UCNACKIFG) && !(UCB0IFG & UCTXIFG) );

    UCB0CTL1 |= UCTXSTP;    					//Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP

    return UCB0IFG & UCNACKIFG;
}



void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
}

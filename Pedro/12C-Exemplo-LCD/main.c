#include <msp430.h> 
#include "../biblioteca/LCD_base.c"
#include "../biblioteca/gpio.c"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	UNLOCK_MASTER_PIN;

	config_I2C();
	LCD_inic();

	PCF_write(0x41);
	PCF_write(0x45);
	PCF_write(0x41);

	PCF_write(0x19);
	PCF_write(0x1D);
	PCF_write(0x19);

	while(1);

}

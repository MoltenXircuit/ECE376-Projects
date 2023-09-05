// Analog Inputs.C


// Global Variables
const unsigned char MSG1[16] = "            ";
const unsigned char MSG2[16] = "           ";

unsigned char TABLE[4] = {1, 2, 4, 8};

// Subroutine Declarations
#include <pic18.h>
#include "LCD_PortD.C"

unsigned int A2D_Read(unsigned char c){
	unsigned int result;
	unsigned char i;
	c = c & 0x0F;
	ADCON0 = (c << 2) + 0x01; // set Channel Select
	for (i=0; i<3; i++); // wait 2.4us (approx)
	 GODONE = 1; // start the A/D conversion
	while(GODONE); // wait until complete (approx 8us)
	return(ADRES);
}


// --------------- main routine ---------------
void main(void)
{
	signed int A2D;
	TRISC = 0;
	ADCON1 = 0x0F;
// Turn on the A/D input
	TRISA = 0xFF;
	TRISE = 0x0F;
	ADCON2 = 0x95;
	ADCON1 = 0x07;
	ADCON0 = 0x01;
	while(1) {
	 A2D = A2D_Read(0);
	 PORTC = PORTC + 1;
	}
}

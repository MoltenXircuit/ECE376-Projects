// ---  HW4a.C -------------------
// base code used for HW Q1
// 

// Global Variables

// Subroutine Declarations
#include <pic18.h>
#include <math.h>

  
// Main Routine



void main(void)
{

	TRISA = 1;
	TRISB = 1;
	TRISC = 0;
	TRISD = 1;
	TRISE = 1;
	ADCON1 = 0x0F;
	PORTA = 1;

	int i;
	float A, B, C;
	A = 3.14159265379;

	while(1) {
	i = (i + 1)% 8;
	if(i == 0) PORTC += 1;
	C = cos(A);
	}
	
}

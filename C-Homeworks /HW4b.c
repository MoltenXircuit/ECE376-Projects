// ---  HW4b.C -------------------
// base code used for HW Q2,Q3
// 

// Global Variables

// Subroutine Declarations
#include <pic18.h>
//#include <math.h>

void Wait(unsigned int DATA)
{
   unsigned int i, j;
   for (i=0; i<DATA; i++) {
      for (j=0; j<765; j++);
   }
}

void Beep(void)
{
	unsigned int a;
	for(a=0;a<=19;a++){
		PORTC =1;
		Wait(2);
		PORTC =0;
		Wait(2);
	}
}
  
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
					//end of intialization
	unsigned int a;
	for(a=0;a<=100;a++){
		Beep();
	}	
	while(1){
	Wait(100);
	}
}

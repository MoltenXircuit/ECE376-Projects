// ---  HW4c.C -------------------
// base code used for HW Q4-Q7
// 

// Global Variables

const unsigned char MSG0[21] = "Roulette.C          "; 
const unsigned char MSG1[21] = "                    "; 

//----------Subroutine Declarations------------
#include <pic18.h>
//#include <math.h>

//---------------Subroutines------------------
#include        "lcd_portd.c"

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
  
//------------Main Routine-------------

void main(void)
{

	TRISA = 0xFF;
	TRISB = 0xFF;
	TRISC = 0;
	TRISD = 0;
	TRISE = 1;
	ADCON1 = 0x0F;

	int Money, Bet, N, i, a;
	Money = 10;
	LCD_Init();                  // initialize the LCD

	LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
	LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]);
	Wait_ms(1000);
	LCD_Inst(1);
	LCD_Move(0,0);	LCD_Write('$');
	LCD_Move(1,0);	LCD_Write('B');
	LCD_Move(1,1);	LCD_Write('e');
	LCD_Move(1,2);	LCD_Write('t');

	LCD_Move(1,7);	LCD_Write('C');
	LCD_Move(1,8);	LCD_Write('u');
	LCD_Move(1,9);	LCD_Write('r');
	LCD_Move(1,10);	LCD_Write('r');
	LCD_Move(1,11);	LCD_Write('e');
	LCD_Move(1,12);	LCD_Write('n');
	LCD_Move(1,13);	LCD_Write('t');
	Beep();
//	Bet = 5;
//	N = 2;
	Wait_ms(1000);

					//end of intialization
	while(1)
	{
		LCD_Move(0,1);	LCD_Out(Money,2,0);	//displays current money
//		LCD_Move(1,3);	LCD_Out(Bet,1,0);	//displays current bet
//		LCD_Move(1,14);	LCD_Out(N%8,1,0);	//displays current Num
		
		while(PORTB==0){
			Wait(100);
		}
		a=PORTB;
		Bet=0;
		while(a!=0){
		a=a/2;
		Bet++;
		}
		Bet--;
		LCD_Move(1,3);	LCD_Out(Bet,1,0);	//displays current bet		
		
		while(PORTB==1){
			N=(N+1)%8;
		}
	

		for(a=0;a<=40+N;a++){
			Beep();
			LCD_Move(1,14);	LCD_Out(a%8,1,0);	//displays current Num
			Wait_ms(100);
		}

		if(N==Bet){ 
			Money=Money+8;
		}
		else{
			Money--;
		}
	}
}

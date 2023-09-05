// Psychic.C
//
// This program generates a random number from 1..4
// and sends if you are correct to the serial port at 9600 baud
// every time you press RB0-RB3

// Global Variables

const unsigned char MSG0[21] = "Fair Die.C       ";
const unsigned char MSG1[21] = "                 "; 

 // Subroutine Declarations
#include <pic18.h>

// Subroutines
#include        "lcd_portd.c"
    

// Main Routine

void main(void)
{
   int DIE,Correct,Guess;
 
   unsigned int i, j;

   TRISA = 0;
   TRISB = 0x0F;
   TRISC = 0x0F;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   LCD_Init();                  // initialize the LCD

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
   LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]);
   Wait_ms(500);

// Initialize Serial Port to 9600 baud
   TRISC = TRISC | 0xC0;
   TXIE = 0;
   RCIE = 0;
   BRGH = 0;
   BRG16 = 1;
   SYNC = 0;
   SPBRG = 255;
   TXSTA = 0x22;
   RCSTA = 0x90;

while(1) {
	while(!RB0&&!RB1&&!RB2&&!RB3);
	if(RB0=1) Guess = 1;
	if(RB1=1) Guess = 2;
	if(RB2=1) Guess = 3;
	if(RB3=1) Guess = 4;
	while(RB0||RB1||RB2||RB3) DIE = (DIE + 1) % 4;
	DIE += 1;
	if (DIE==Guess) Correct= 1;
	else Correct = 0;
	LCD_Move(1,0); LCD_Out(DIE, 1, 0);
	LCD_Move(1,8); LCD_Out(Guess, 1, 0);
	SCI_Out(Correct, 1, 0);
	SCI_CRLF();
	}      
}

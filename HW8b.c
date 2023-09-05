// HW8b

// Global Variables

const unsigned char MSG0[20] = "T2 Banjo         ";
const unsigned char MSG1[20] = "                   ";
unsigned int COUNT = 0;
unsigned long int TIME;

// Subroutine Declarations
#include <pic18.h>
#include "lcd_portd.c"


void interrupt IS(void) 
{
 if (TMR2IF) {
    COUNT++;
    if(COUNT==4){
	    if(RB0) RC0 = !RC0;
	    COUNT = 0;
				}
//    if(TIME != 0) TIME -= 1;
    TMR2IF = 0;
    }
 }

// Main Routine
void main(void)
{
 unsigned char i, j;
 TRISA = 0;
 TRISB = 0xFF;
 TRISC = 0;
 TRISD = 0;
 TRISE = 0;
 ADCON1 = 15;

// Timer2 Initialize
 TMR2ON = 1;
 TMR2IE = 1;
 PEIE = 1;
 T2CON = 0x27;
 PR2 = 213;
 GIE = 1;

 LCD_Init();
 LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]); 
 LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]); 

 TIME = 0;
 
 while(1) {
    asm("nop");
    LCD_Move(1,0); LCD_Out(TIME, 5, 3);
    }    
 }
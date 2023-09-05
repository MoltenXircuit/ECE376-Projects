// HW8a

// Global Variables

const unsigned char MSG0[20] = "T2 Stopwatch       ";
const unsigned char MSG1[20] = "                   ";
//unsigned char RUN;
unsigned long int TIME;

// Subroutine Declarations
#include <pic18.h>
#include "lcd_portd.c"


void interrupt IS(void) 
{
 if (TMR2IF) {
    RC0 = !RC0;
    if(TIME != 0) TIME -= 1;
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
 T2CON = 0x4D;
 PR2 = 249;
 GIE = 1;

 LCD_Init();
 LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]); 
 LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]); 

 TIME = 0;
 
 while(1) {
    if(RB0) TIME = 5000;
    if(RB1) TIME = 10000;
    if(RB2) TIME = 15000;
    if(RB3) TIME = 20000;
    LCD_Move(1,0); LCD_Out(TIME, 5, 3);
    }    
 }
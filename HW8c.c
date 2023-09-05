// HW8c

// Global Variables

const unsigned char MSG0[20] = "Reflex Timer       ";
const unsigned char MSG1[20] = "                   ";

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
 if (TMR0IF) {
    TIME = TIME + 0x10000;
    TMR0IF = 0;
      }
 }

// Main Routine
void main(void)
{
 unsigned char i, j;
 unsigned long int TIME1, TIME2; 
 unsigned char Flag;
 unsigned char Delay=0;
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

// set up Timer0 for PS = 1
  T0CS = 0;
  T0CON = 0x88;
  TMR0ON = 1;
  TMR0IE = 1;
  TMR0IP = 1;
  PEIE = 1;
   
// turn on all interrupts
  GIE = 1;

 LCD_Init();
 LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]); 
 LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]); 

 TIME = 0;
 
 while(1) {
	  while(!RB0){
	    Delay++;
	    }
	  while(RB0){
	    asm("nop");
	    } 
	  Wait_ms(3000+(15*Delay));
	  Flag = RB0;
      PORTA = 255;
      TIME1 = TIME + TMR0;
//	  TIME2 = TIME1;
	  while(!RB0){
		asm("nop");
		}
      TIME2 = TIME + TMR0;
      PORTA = 0;
	  while(RB0){
	    asm("nop");
	    } 
	  if(!Flag){
        LCD_Move(1,0);
        LCD_Out(TIME2 - TIME1, 10, 7);
      }
	  else{
        LCD_Move(1,0);
        LCD_Out(Delay, 3, 0);
      }
    }    
 }
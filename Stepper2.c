// Stepper2.C
#include <pic18.h>

// Global Variables
const unsigned char MSG[16] = "Stepper2.C     ";
unsigned char TABLE[4] = {1, 2, 4, 8};
int STEP, N, TIME;

// Subroutine Declarations
#include "LCD_PortD.C"
        
// Main Routine

void main(void)
{
   unsigned int i, MS, DIR;

   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0;
   ADCON1 = 0x0F;

   DIR = 0;
   MS = 100;
   STEP = 0;

   LCD_Init();
   LCD_Move(0,0);
   for (i=0; i<16; i++) LCD_Write(MSG[i]);
   Wait_ms(100);


   while(1) {
      if(RB0) { DIR = -1;  MS =  30; }
      if(RB1) { DIR = -1;  MS = 100; }
      if(RB2) { DIR =  0;  MS = 100; }
      if(RB3) { DIR =  1;  MS = 100; }
      if(RB4) { DIR =  1;  MS =  30; }

      STEP = STEP + DIR;
      PORTC = TABLE[STEP % 4];
      LCD_Move(1,0);
      LCD_Out(STEP, 5, 0);
      Wait_ms(MS);
      }
   }
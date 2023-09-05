// Stepper1.C
// Drive a stepper motor at 100ms/step

// Global Variables
const unsigned char MSG[16] = "Stepper1.C      ";
unsigned char TABLE[4] = {1, 2, 4, 8};
int STEP, N, TIME;

// Subroutine Declarations
#include <pic18.h>
#include "LCD_PortD.C"
        
// Main Routine

void main(void)
{
   int i;

   TRISA = 0;
   TRISB = 0;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   STEP = 0;

   LCD_Init();
   LCD_Move(0,0);
   for (i=0; i<16; i++) LCD_Write(MSG[i]);
   Wait_ms(100);

   while(1) {
      STEP = STEP + 1;
      PORTC = TABLE[STEP % 4];
      LCD_Move(1,0);
      LCD_Out(STEP, 5, 0);
      Wait_ms(100);
      }
  
 }      

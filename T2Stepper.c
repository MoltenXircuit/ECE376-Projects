// T2 Stepper
// Drive a stepper motor

// Global Variables
const unsigned char MSG[16] = "T2 Stepper      ";
unsigned char TABLE[4] = {1, 2, 4, 8};
int STEP, N, TIME;

// Subroutine Declarations
#include <pic18.h>
#include "LCD_PortD.C"

// High-priority service
void interrupt IntServe(void)
{
   if (TMR2IF) {
      N = (N + 1) % 100;
      if (N == 0) {
         STEP = STEP + 1;
         PORTC = TABLE[STEP %  4];
         RA1 = !RA1;
         }
      TMR2IF = 0;
      }
   }


        
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

// initialize Timer2 for 1ms

   T2CON = 0x4C;
   PR2 = 199;
   TMR2ON = 1;
   TMR2IE = 1;
   PEIE = 1;

   GIE = 1;

   while(1) {
      LCD_Move(1,0);   LCD_Out(STEP, 5, 0);
      }
  
 }      

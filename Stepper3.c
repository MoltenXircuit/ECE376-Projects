// Stepper3.C
// Position control of a stepper motor

// Global Variables
const unsigned char MSG1[16] = "REF             ";
const unsigned char MSG2[16] = "STEP            ";

unsigned char TABLE[4] = {1, 2, 4, 8};

// Subroutine Declarations
#include <pic18.h>
#include "LCD_PortD.C"
        

// main routine
void main(void) {
   unsigned int i, REF, STEP ;

   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0;
   ADCON1 = 0x0F;

   STEP = 0;
   REF   = 100;

   LCD_Init();
   LCD_Move(0,0);  for (i=0; i<16; i++) LCD_Write(MSG1[i]);
   LCD_Move(1,0);  for (i=0; i<16; i++) LCD_Write(MSG2[i]);
   Wait_ms(15);

   while(1) {
      if(RB0) REF = 0;
      if(RB1) REF = 25;
      if(RB2) REF = 50;
      if(RB3) REF = 75;
      if(RB4) REF = 100;

      if (STEP < REF) STEP = STEP + 1;
      if (STEP > REF) STEP = STEP - 1;

      PORTC = TABLE[STEP % 4];
      LCD_Move(0,8);  LCD_Out(REF, 5, 0);
      LCD_Move(1,8);  LCD_Out(STEP, 5, 0);
      Wait_ms(30);
      }
   }
   


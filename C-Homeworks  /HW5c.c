// Stepper NotLidar.C

// Global Variables
const unsigned char MSG1[16] = "MAX             ";
const unsigned char MSG2[16] = "NOW             ";

unsigned char TABLE[4] = {1, 2, 4, 8};

// Subroutine Declarations
#include <pic18.h>
#include <math.h>
#include  "lcd_portd.c"

   
unsigned int A2D_Read(unsigned char c)
{
   unsigned int result;
   unsigned char i;
   c = c & 0x0F;
   ADCON0 = (c << 2) + 0x01; // set Channel Select
   for (i=0; i<20; i++); // wait 2.4us (approx)
   GODONE = 1; // start the A/D conversion
   while(GODONE); // wait until complete (approx 8us)
   return(ADRES);
   }   


// --------------- main routine ---------------
void main(void) {
   int A2D;     // raw A/D readings
   float LUX, MaxLux;
   unsigned int i;
   signed int REF, STEP, velocity, direction, MaxStep;

   TRISA = 0xFF;
   TRISE = 0x0F;
   TRISB = 0xFF;
   TRISC = 0;
   ADCON2 = 0x85;
   ADCON1 = 0x07;
   ADCON0 = 0x01;
   PORTC = 0;
   STEP = 1;
   REF = 51;
   velocity = 80; //max velocity=100
   direction = 1;
   
   LCD_Init();
   LCD_Move(0,0);  for (i=0; i<16; i++) LCD_Write(MSG1[i]);
   LCD_Move(1,0);  for (i=0; i<16; i++) LCD_Write(MSG2[i]);
   Wait_ms(500);

   while(1) {
	  A2D = A2D_Read(1);
	  LUX = ( (1023.0 - A2D) / A2D ) * 100;
	  LCD_Move(1,12);  LCD_Out(LUX, 3, 0);	//output current step+lux
      LCD_Move(1,6);  LCD_Out(STEP*1.8, 2, 0);
      	  
      if(RB0) REF = 0;
      if(RB1) REF = 51;
      if(MaxLux < LUX){
        MaxLux = LUX;
        MaxStep = STEP;
      }  

	  if((STEP >= 50)|(STEP <= -50)){				//at +/- 90 deg reverse direction
		  direction = 0-direction;
	      LCD_Move(0,12);  LCD_Out(MaxLux, 3, 0);	//output max step+lux
	      LCD_Move(0,6);  LCD_Out(MaxStep*1.8, 2, 0);	//then reset max lux
		  MaxLux = 0;
		 } 
      if (STEP != REF) STEP = STEP + direction;
      PORTC = TABLE[STEP % 4];
//	  LCD_Move(0,6);  LCD_Out(MaxStep, 2, 0);
//	  LCD_Move(0,12);  LCD_Out(MaxLux, 3, 0);
      Wait_ms(100-velocity); 
      }

}

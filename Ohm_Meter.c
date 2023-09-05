// Ohm_Meter.C
//
// This program reads the A/D on RA0
// Computes the voltage

// Global Variables

const unsigned char MSG0[21] = "Ohm_Meter.C        "; 
 

// Subroutines
#include <pic18.h>
#include <math.h>
#include        "lcd_portd.c"

   
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


// Main Routine

void main(void)
{
   int A2D;     // raw A/D readings
   float VOLT;    // Volts*100 (500 means 5.00V)
   float OHM;	 // resisance (Ohms)
   float CELSIUS; // temperature*100
   float LUX;

   unsigned int i, j, DIE;

   TRISA = 0;
   TRISB = 0x0F;
   TRISC = 0x0F;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   LCD_Init();                  // initialize the LCD

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
   Wait_ms(500);

   LCD_Inst(0x01);

// Initialize the A/D port
   TRISA = 0xFF;
   TRISE = 0x0F;
   ADCON2 = 0x85;
   ADCON1 = 0x07;
   ADCON0 = 0x01;
   i = 0;

   while(1) {

      A2D = A2D_Read(1);

	  VOLT = 0.488 * A2D;
	  OHM = 1000.0 * (A2D / (1023.0 - A2D) );
	  CELSIUS = 39300. / ( log( A2D / (1023. - A2D) ) + 13.1879 ) - 2730;
	  LUX = ( (1023.0 - A2D) / A2D ) * 100;

      LCD_Move(0,0);  LCD_Out(A2D, 5, 0);
      LCD_Move(0,8);  LCD_Out(VOLT, 5, 0);
      LCD_Move(1,0);  LCD_Out(OHM, 5, 0);
      LCD_Move(1,8);  LCD_Out(LUX, 5, 0);
  
      Wait_ms(100);

      }      
   }
// Logger.C
//
// Data Logger
// This program reads the A/D on RA0
// and sends it to the serial port at 9600 baud

// Global Variables

const unsigned char MSG0[21] = "Logger.C            "; 

 // Subroutine Declarations
#include <pic18.h>

// Subroutines
#include        "lcd_portd.c"
   

unsigned int A2D_Read(unsigned char c)
{
   unsigned int result;
   unsigned char i;
   c = c & 0x0F;
   ADCON0 = (c << 2) + 0x01; 
   for (i=0; i<20; i++); 
   GODONE = 1; 
   while(GODONE); 
   return(ADRES);
   }   
     

// Main Routine

void main(void)
{
   int A2D;     // raw A/D readings
   int VOLT;    // Volts*100 (500 means 5.00V)
 
   unsigned int i, j;

   TRISA = 0;
   TRISB = 0x00;
   TRISC = 0x0F;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   LCD_Init();                  // initialize the LCD

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
   Wait_ms(500);

// Initialize the A/D port
   TRISA = 0xFF;
   TRISE = 0x0F;
   ADCON2 = 0x85;
   ADCON1 = 0x07;
   ADCON0 = 0x01;
   i = 0;

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

      A2D = A2D_Read(1);

      VOLT = 0.488*A2D ;

//      LCD_Move(0,10);  LCD_Out(VOLT, 5, 2);
 
      SCI_Out(VOLT, 5, 2);
      SCI_CRLF();
  	  PORTB += 1;
      Wait_ms(10);

      }      
   }


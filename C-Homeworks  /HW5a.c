// HW5a.C

// Global Variables

unsigned char PIXEL @ 0x000;

const unsigned char MSG0[20] = "Current            ";
const unsigned char MSG1[20] = "Desired            ";

// Subroutine Declarations
#include <pic18.h>

// --------------- Subroutines ---------------
#include        "lcd_portd.c"
// --------------- Keypad Subs ---------------
char GetKey(void)
{
   int i;
   unsigned char RESULT;
   TRISC = 0xF8;
   RESULT = 0xFF;
   PORTC = 4;
   for (i=0; i<100; i++);
   if (RC6) RESULT = 1;
   if (RC5) RESULT = 4;
   if (RC4) RESULT = 7;
   if (RC3) RESULT = 10;
   PORTC = 2;
   for (i=0; i<100; i++);
   if (RC6) RESULT = 2;
   if (RC5) RESULT = 5;
   if (RC4) RESULT = 8;
   if (RC3) RESULT = 0;
   PORTC = 1;
   for (i=0; i<100; i++);
   if (RC6) RESULT = 3;
   if (RC5) RESULT = 6;
   if (RC4) RESULT = 9;
   if (RC3) RESULT = 11;
   PORTC = 0;
   return(RESULT);
}
      
char ReadKey(void)
{
   char X, Y;
   do {
      X = GetKey();
      }   while(X > 20);
   do {
      Y= GetKey();
      }   while(Y < 20);
   Wait_ms(100);  // debounce
   return(X);
   }

// --------------- Neopixel Subs ---------------
void NeoPixel_Display(unsigned char RED, 
		unsigned char GREEN, unsigned char BLUE)
{
   PIXEL = GREEN;	asm("  call Pixel_8 ");
   PIXEL = RED;		asm("  call Pixel_8 ");
   PIXEL = BLUE;	asm("  call Pixel_8 ");

   asm("    return");


#asm
Pixel_8:
    call	Pixel_1
    call	Pixel_1
    call	Pixel_1
    call	Pixel_1
    call	Pixel_1
    call	Pixel_1
    call	Pixel_1
    call	Pixel_1
    return

Pixel_1:
	bsf		((c:3971)),0	; PORTD,0
    nop
	btfss   ((c:0000)),7
	bcf		((c:3971)),0
	rlncf   ((c:0000)),F
    nop
    nop
    bcf		((c:3971)),0
    return

#endasm
   }

         
// --------------- Main Routine ---------------

void main(void)
{
   unsigned int i, N, TEMP;
   unsigned int bright = 0;
   unsigned int tempBright = 0;
   unsigned int num1, num2, num3 = 0;
   unsigned char RED, GREEN, BLUE;

   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0xF8;
   TRISD = 0;
   TRISE = 0;
   TRISA = 0;
   PORTB = 0;
   PORTC = 0;
   PORTD = 0;
   PORTE = 0;
   ADCON1 = 0x0F;

   LCD_Init();                  // initialize the LCD
   TRISD0 = 0;

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]); 
   LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]); 
   Wait_ms(100); 

   while(1) {
// --------------- Input ---------------
    TEMP = ReadKey();
    if (TEMP < 10){
	 num1 = num2;
	 num2 = num3;
	 num3 = TEMP;
	 tempBright = (100*num1)+(10*num2)+num3;
     LCD_Move(1,10);  LCD_Out(tempBright, 3, 0);
	}
	
    if (TEMP == 11) {
	 bright = tempBright;
	 if (bright>255) bright=255;
    }

// --------------- Output ---------------

  for (i=0; i<8; i++) {
   NeoPixel_Display(bright, bright, bright);
   }
   LCD_Move(0,10);  LCD_Out(bright, 3, 0);
   Wait_ms(100); 
}
}   

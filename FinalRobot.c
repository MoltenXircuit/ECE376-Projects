
// Global Variables

const unsigned char MSG0[21] = "Robot             "; 
const unsigned char MSG1[21] = "                    ";     
const unsigned char MSG2[21] = "MAX                 ";
const unsigned char MSG3[21] = "NOW                 ";

unsigned char TABLE[8] = {8, 12, 4, 6, 2, 3, 1, 9};
//unsigned int REF=0, STEP=0;
	signed int REF=0, STEP=0, velocity, vcount, direction, MaxStep;
//unsigned char BeepEnable, Beep;
unsigned long int TIME;

// Subroutine Declarations
#include <pic18.h>

// Subroutines
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
   
void CountNum(void) {
	REF = REF+25;
	while(REF!=STEP);
    LCD_Move(1,0);  LCD_Out(TIME+TMR1, 10, 7);
}


// ---------------interrupt service routine ---------------
void interrupt IntServe(void)
{
   if (TMR0IF) {
      TMR0 = -25000;
      if (vcount == 0){
	      vcount = 100-velocity;
	      if (STEP != REF) STEP = STEP + direction;      
	     }
	  else vcount-- ;
      PORTC = TABLE[STEP % 8];      
      TMR0IF = 0;
      }
/*   if (TMR1IF) {
      TIME = TIME + 0x10000;
      TMR1IF = 0;
      }
   if (TMR2IF) {
	  if(Beep) {
		 BeepEnable = 1;
		 BeepLength = 100;
		 Beep = 0;
		 }
	  if(BeepLength>0) {
		 BeepLength--;
		 }
	  else BeepEnable = 0;
      TMR2IF = 0;
      }   
   if (TMR3IF) {
      TMR3 = -29494;
      if (BeepEnable) RE0 = !RE0;
      TMR3IF = 0;
      }*/
   }
 

         
// ---------------initialization routine ---------------
void main(void)
{
   unsigned int i, CurrentNum, N;
   unsigned long int Timer1, Timer2;
   TRISA = 0;
   TRISB = 0b11111111;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;
   
   STEP = 0;
   REF = 100;
   velocity = 99; //max velocity=100
   direction = 1;


   LCD_Init();                  // initialize the LCD

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
   LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]);

   Wait_ms(100);
   
// --------------- interrupt setup ---------------
// set up Timer0 for PS = 4
	T0CS = 0;
	T0CON = 0x81;
	TMR0ON = 1;
	TMR0IE = 1;
	TMR0IP = 1;
	PEIE = 1;
// set up Timer1 for PS = 1
/*	TMR1CS = 0;
	T1CON = 0x81;
	TMR1ON = 1;
	TMR1IE = 1;
	TMR1IP = 1;
	PEIE = 1;
// set up Timer2 for 1ms
	T2CON = 0x4D;
	PR2   = 249;
	TMR2ON = 1;
	TMR2IE = 1;
	TMR2IP = 1;
	PEIE = 1;
// set up Timer3 for PS = 1
	TMR3CS = 0;
	T3CON = 0x81;
	TMR3ON = 1;
	TMR3IE = 1;
	TMR3IP = 1;
	PEIE = 1;
// turn on all interrupts			*/
GIE = 1;

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG2[i]);
   LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG3[i]);
// Initialize Serial Port to 9600 baud
/*   TRISC = TRISC | 0xC0;
   TXIE = 0;
   RCIE = 0;
   BRGH = 0;
   BRG16 = 1;
   SYNC = 0;
   SPBRG = 255;
   TXSTA = 0x22;
   RCSTA = 0x90;			*/
 // --------------- main loop ---------------  
   while(1) {
/*	  A2D = A2D_Read(1);
	  LUX = ( (1023.0 - A2D) / A2D ) * 100;
	  LCD_Move(1,12);  LCD_Out(LUX, 3, 0);	//output current step+lux
*/      LCD_Move(1,6);  LCD_Out(STEP*18/10, 2, 0);
      	  
      if(RB0) REF = 0;
      if(RB1) REF = 100;
/*      if(MaxLux < LUX){
        MaxLux = LUX;
        MaxStep = STEP;
      }  
*/
//	  if((STEP >= 50)(STEP <= -50)){				//at +/- 90 deg reverse direction
	  if(STEP >= 50) direction = -1;
	  if(STEP <= -50) direction = 1;

//	      LCD_Move(0,12);  LCD_Out(MaxLux, 3, 0);	//output max step+lux
//	      LCD_Move(0,6);  LCD_Out(MaxStep*1.8, 2, 0);	//then reset max lux
//		  MaxLux = 0;
//		 } 
//      if (STEP != REF) STEP = STEP + direction;
//	  LCD_Move(0,6);  LCD_Out(MaxStep, 2, 0);
//	  LCD_Move(0,12);  LCD_Out(MaxLux, 3, 0);
      
//	 SCI_Out(N, 1, 0);
//	 SCI_CRLF();
     }      
}


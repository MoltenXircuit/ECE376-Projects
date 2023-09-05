
// Global Variables

const unsigned char MSG0[21] = "HW9e.C             "; 
const unsigned char MSG1[21] = "                    ";     

unsigned char TABLE[4] = {8, 4, 2, 1};
unsigned int REF=0, STEP=0, BeepLength;
unsigned char BeepEnable, Beep;
unsigned long int TIME;

// Subroutine Declarations
#include <pic18.h>

// Subroutines
#include        "lcd_portd.c"


void CountNum(void) {
	REF = REF+25;
	while(REF!=STEP);
    LCD_Move(1,0);  LCD_Out(TIME+TMR1, 10, 7);
}


// Interrupt Service Routine
void interrupt IntServe(void)
{
   if (TMR0IF) {
      TMR0 = -25000;
      if (STEP < REF) STEP = STEP + 1;
      if (STEP > REF) STEP = STEP - 1;
      PORTC = TABLE[STEP % 4];      
      TMR0IF = 0;
      }
   if (TMR1IF) {
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
      }
   }
 

         
// Main Routine

void main(void)
{
   unsigned int i, CurrentNum, N;
   unsigned long int Timer1, Timer2;
   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   LCD_Init();                  // initialize the LCD

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
   LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]);

   Wait_ms(100);

// set up Timer0 for PS = 4
   T0CS = 0;
   T0CON = 0x81;
   TMR0ON = 1;
   TMR0IE = 1;
   TMR0IP = 1;
   PEIE = 1;
// set up Timer1 for PS = 1
   TMR1CS = 0;
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
// turn on all interrupts
GIE = 1;

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
	 while(RB0);
	 while(!RB0){
		if(RB2) {
			Timer1 = TIME+TMR1;
			Beep = -1;
			while(BeepEnable != 0);
			Timer2 = TIME+TMR1;
    		LCD_Move(1,0);  LCD_Out(Timer2-Timer1, 10, 7);				
		}	
		if(RB1){
			REF = REF%200;
			STEP = STEP%200;
			REF = 0;
			N = 0;
		}  
		LCD_Move(0,6);  LCD_Out(N, 5, 0);
     	LCD_Move(1,0);  LCD_Out(TIME+TMR1, 10, 7);
  	 }
	 N = (TIME + TMR1)%8;//+24;
	 while(CurrentNum != N){
		CountNum();
		CurrentNum++;
		Beep = 1;
		LCD_Move(0,6);  LCD_Out(CurrentNum%8, 5, 0);
    	LCD_Move(1,0);  LCD_Out(TIME+TMR1, 10, 7);
	 }
//	 N=N-24;
	 CurrentNum=0;
	 SCI_Out(N, 1, 0);
	 SCI_CRLF();
     }      
   }


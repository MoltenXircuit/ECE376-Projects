
// Global Variables

const unsigned char MSG0[21] = "Robot             "; 
const unsigned char MSG1[21] = "                    ";     
const unsigned char MSG2[21] = "MAX                 ";
const unsigned char MSG3[21] = "NOW                 ";

unsigned char TABLE_R[16] = { 0, 32, 0, 8, 0, 4, 0, 16, 0, 8, 0, 32, 0, 16, 0, 4};
unsigned char TABLE_L[16] = { 0, 4, 0, 16, 0, 8, 0, 32, 0, 16, 0, 4, 0, 32, 0, 8};
signed int REF=0, STEP=0, velocity, vcount, direction, MaxStep, MinStep, LastStep, StepSizeL, StepSizeR, CenterStep;
unsigned long int TIME, TIME0, TIME1, dT;
//unsigned char BeepEnable, Beep;
//unsigned long int TIME;

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
   


// ---------------interrupt service routine ---------------
void interrupt IntServe(void)
{
   if (TMR0IF) {
      TMR0 = -25000;
      if (vcount == 0){
	      vcount = 100-velocity;
	      if (STEP != REF) STEP = STEP + direction;
	      if (direction == 1) PORTC = PORTC ^ TABLE_R[(((2*(STEP%8))+direction) % 16)];
	      if (direction == -1) PORTC = PORTC ^ TABLE_L[(((2*(STEP%8))+direction) % 16)];
	     }
	  else vcount-- ;
      TMR0IF = 0;
      }
   if (TMR1IF) {
      TIME = TIME + 0x10000;
      TMR1IF = 0;
      }
   if (CCP2IF) {
	 if (CCP2CON == 0x05) { // rising edge
		TIME0 = TIME + CCPR2;
		CCP2CON = 0x04;
		}
	 else {
		TIME1 = TIME + CCPR2;
		dT = TIME1 - TIME0;
		CCP2CON = 0x05;
//   		RC0 = 1;
   		RC6 = !RC6;
   		}
	 CCP2IF = 0;
   }
/* if (TMR2IF) {
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
   int A2D;     // raw A/D readings
   unsigned int i, CurrentNum, N;
   float LUX;
   unsigned long int Timer1, Timer2;
   unsigned int Distance, MaxDistance, MinDistance;
   TRISA = 0;
   TRISB = 0b11111111;
   TRISC = 0b00000010;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;
   PORTC = 0b00000101;
//-------------------initialize walking values------------
   CenterStep = 0;
   STEP = 0;
   REF = 100;
   velocity = 99; //max velocity=100
   direction = 1;


   LCD_Init();                  // initialize the LCD

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
   LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]);

   Wait_ms(1000);
   
// --------------- interrupt setup ---------------
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
/*	T2CON = 0x4D;
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
	PEIE = 1;			*/
// set up Capture2 for rising edges
//	TRISC1 = 1;
	CCP2CON = 0x05;
	CCP2IE = 1;
	PEIE = 1;
// turn on all interrupts
	GIE = 1;

// Initialize the A/D port
   TRISA = 0xFF;
   TRISE = 0x0F;
   ADCON2 = 0x85;
   ADCON1 = 0x07;
   ADCON0 = 0x01;
   i = 0;

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
      StepSizeL = 50-CenterStep;
      StepSizeR = 50-CenterStep;
	  Distance = dT * 0.1715; // 1 count = 1/10 mm
	  A2D = A2D_Read(0);
	  LUX = A2D;
	  LCD_Move(1,8);  LCD_Out(Distance, 7, 0);	//output current step+Distance
// 	  LCD_Move(1,8);  LCD_Out(LUX, 7, 0);	//output current step+Distance
      LCD_Move(1,4);  LCD_Out(STEP*0.9, 2, 0);

//	Button Controls
      if(RB0) REF = 0;
      if(RB1) REF = 100;
      if(RB7) TMR0ON = 0;
      if(RB6) TMR0ON = 1;
//      if(RB2) RC0 = 0;


//	Reset Distance Sensor
	  if((STEP%5 != 0)&&(RC0 == 0)) RC0 = 1;
//	Measure Distance w/ Sensor
	  if((STEP%5 == 0)&&(RC0 == 1)) RC0 = 0;

//	Robot heartbeat
//      if(TIME%500 == 0) RC0 = !RC0;

      if(MaxDistance < Distance){
        MaxDistance  = Distance;
        MaxStep = STEP;
      }
     if(MinDistance > Distance){
        MinDistance  = Distance;
        MinStep = STEP;
      }

//	Switch to left move
	  if(STEP >= StepSizeR) {
		direction = -1;
		}

//	Switch to right move
	  if(STEP <= -StepSizeL) {
		direction = 1;
		}


//	Light Sensor Speed Control
      if(LUX<300) {
	      velocity = 0;
	      TMR0ON = 0;
	      OSCCON = 1;
	      while(LUX<300) LUX = A2D_Read(0);
	      OSCCON = 1;
	      TMR0ON = 1;
	      }
      else if(LUX<800) velocity = 100;
      else if(LUX<300) velocity = 90+(LUX/50)-6;



	  if(((STEP >= StepSizeR)||(STEP <= -StepSizeL))&&(LastStep !=STEP)){				//at +/- 90 deg reverse direction
		LCD_Move(0,8);  LCD_Out(MaxDistance, 7, 0);	//output max step+distance
		LCD_Move(0,4);  LCD_Out(MaxStep*0.9, 2, 0);
		CenterStep = MaxStep/9;
		MaxDistance = 0;		//then reset max distance		
		}
	  LastStep = STEP;
      
//	 SCI_Out(N, 1, 0);
//	 SCI_CRLF();
     }      
}


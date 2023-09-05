; 1234.asm
; Display the numbers {1,2,3,4} on
; {PORTA, PORTB, PORTC, PORTD}
 
#include <p18f4620.inc> 

      org 0x800
      clrf TRISA
      clrf TRISB
      clrf TRISC
      clrf TRISD
      movlw 0x0F
      movwf ADCON1

      movlw 1
      movwf PORTA
      movlw 2
      movwf PORTB
      movlw 3
      movwf PORTC
      movlw 4
      movwf PORTD

Loop:
      goto Loop
      end

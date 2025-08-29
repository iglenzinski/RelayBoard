#include "Errors.h"

ErrorModule::ErrorModule(){
  DDRB |= (1 << PB7 );
  PORTB &= ~(1 << PB7 );
}

void ErrorModule::BlinkInitErrorLoop(int ErrorCode){
  while(1)
  {
    for (int i = 0; i < ErrorCode; i++)
    {
      PORTB |= (1 << PB7 );
      delay(400);
      PORTB &= ~(1 << PB7 );
      delay(400);
    }
    delay(1500);
  }
}
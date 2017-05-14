#include <pins.h>

using namespace pins;

pin<13> LED;
pin<0> SWITCH;

void setup() {
  LED.pinMode(OUTPUT);
  SWITCH.pinMode(INPUT_PULLUP); 
}

void loop() 
{    
  while(SWITCH == LOW)
  {
    LED.toggle();
    delay(250);
  }
}

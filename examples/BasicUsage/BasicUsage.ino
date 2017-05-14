#include <pins.h>

using namespace pins;

pin<13> LED(OUTPUT);
pin<0> SWITCH(INPUT_PULLUP);

void setup() {  
}

void loop() 
{    
  while(SWITCH == LOW)
  {
    LED.toggle();
    delay(250);
  }
}

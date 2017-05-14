#include <pins.h>
using namespace pins;

pin<13> LED;

void setup() {
  LED.pinMode(OUTPUT);
}

void loop() {
  LED.toggle();
  delay(250);
}

# pins
This library is provides an alternative to the usual pin access by pinMode, digitalWriteFast and digitalReadFast for Teensy ARM boards (T-LC up to T-3.6). Generated code is as efficient as calling the standard pin manipulation functions. 

# Usage
The following sketch shows the basic usage of the pin class
```c++
#include <pins.h>
using namespace pins;

pin<13> LED(OUTPUT);  // Assignes the variable LED to pin 13 and switches its pin mode to OUTPUT

void setup(){         
    LED = HIGH;       // Switch LED on
    LED = LOW;        // .. and off
    LED.toggle();     // toggle LED
}

void loop(){
}
```
You can also do things like
```c++
...
pin<0>  SWITCH(INPUT_PULLUP);   // Assume we have a switch attached to pin 0
pin<13> LED(OUTPUT);            // LED on pin 13
...
void loop()
{
    while(SWITCH == LOW){
        LED.toggle();
        delay(100);
    }
}
```
## Advanced Features
You can also use the static interface of the pin class directly:
```c++
pin<3>() = HIGH; 
pin<4>::toggle();
pin<5>::pinMode(OUTPUT_OPENDRAIN);
```
Attaching an interrupt to a pin: 
```c++
SWITCH.attachInterrupt(callback, RISING);   
```
Change pin properties:
```c++
LED.driveStrengthEnable(false);      // which would be a bad idea of course...   
pin<7>::slowSlewRateEnable(false);   // enables full slew rate for the pin
```

If you need to change the pin assignment, e.g. you want to move Serial1 from the standard pins 0 and 1 to its alternative pins 21 and 5 (those pins which are grayed out on the pinout cards) you can use:
```c++
pin<21>::setMUX(ALT3);  // Set alternative function of pin21 to ALT3
pin<5>::setMUX(ALT3);   // Set alternative function of pin5 to ALT3
```
(see the datasheet for a list of possible alternative pin functions)

# Performance
Reading and writing 
```c++
LED = HIGH; 
bool b = SWITCH; 
```
is as fast as digitalWriteFast() and digitalReadFast(). Toggling 
```c++
LED.toggle();
```
is faster than the usual 
```c++
digitalWriteFast(13,!digitalReadFast(13));
```
### Comparison
Paul hast demonstrated how extremly fast *digitalWriteFast()* really is in this post: https://forum.pjrc.com/threads/41874-STRANGE-GPIO-speed-perfomance-with-digitalWriteFast()-(ring-oscillator-example)?p=132359&viewfull=1#post132359.

Here the standard code and the generated machine instructions:
```c++
void loop() {
    while (1) {
        digitalWriteFast(14, HIGH);
        digitalWriteFast(14, LOW);
        digitalWriteFast(14, HIGH);
        digitalWriteFast(14, LOW);
        digitalWriteFast(14, HIGH);
        digitalWriteFast(14, LOW);
        digitalWriteFast(14, HIGH);
        digitalWriteFast(14, LOW);
    }
}

00002fe8 <loop>:
    2fe8:	ldr	r1, [pc, #20]	; (3000 <loop+0x18>)
    2fea:	movs	r3, #2
    2fec:	ldr	r2, [pc, #20]	; (3004 <loop+0x1c>)
    2fee:	str	r3, [r1, #0]
    2ff0:	str	r3, [r2, #0]
    2ff2:	str	r3, [r1, #0]
    2ff4:	str	r3, [r2, #0]
    2ff6:	str	r3, [r1, #0]
    2ff8:	str	r3, [r2, #0]
    2ffa:	str	r3, [r1, #0]
    2ffc:	str	r3, [r2, #0]
    2ffe:	b.n	2fee <loop+0x6>
    3000:	.word	0x400ff0c4
    3004:	.word	0x400ff0c8
```
And here the same for the *pin* interface:
```c++
pin<14> Pin;
void loop() {
    while (1) {
        Pin = HIGH;
        Pin = LOW;
        Pin = HIGH;
        Pin = LOW;
        Pin = HIGH;
        Pin = LOW;
        Pin = HIGH;
        Pin = LOW;
    }
}
    00002fe8 <loop>:
    2fe8:	ldr	r3, [pc, #20]	; (3000 <loop+0x18>)
    2fea:	movs	r1, #1
    2fec:	movs	r2, #0
    2fee:	str	r1, [r3, #0]
    2ff0:	str	r2, [r3, #0]
    2ff2:	str	r1, [r3, #0]
    2ff4:	str	r2, [r3, #0]
    2ff6:	str	r1, [r3, #0]
    2ff8:	str	r2, [r3, #0]
    2ffa:	str	r1, [r3, #0]
    2ffc:	str	r2, [r3, #0]
    2ffe:	b.n	2fee <loop+0x6>
    3000:	.word	0x43fe1804
```
Which should have the same performance


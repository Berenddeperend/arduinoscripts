#include <LowPower.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Wtv020sd16p.h>

const int resetPin = 3;  // The pin number of the reset pin.
const int clockPin = 4;  // The pin number of the clock pin.
const int dataPin  = 5;  // The pin number of the data pin.
const int busyPin  = 6;  // The pin number of the busy pin.

const int MAX_VOICES = 356;

const int wakeUpPin = 2;

unsigned long clock = 0;

/*
Create an instance of the Wtv020sd16p class.
1st parameter: Reset pin number.
2nd parameter: Clock pin number.
3rd parameter: Data pin number.
4th parameter: Busy pin number.
*/
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

void setup() {
  randomSeed(analogRead(0));

  
 //Initializes the module.
 wtv020sd16p.reset();
//  wtv020sd16p.setVolume(2);

 /* Setup the sleep pin direction. */
  pinMode(wakeUpPin, INPUT_PULLUP);
}

int voice = 0;

void loop() {
    
    int clock = millis();
    randomSeed(clock);

    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, LOW);
    
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0); 
    
    //Plays asynchronously an audio file.
    wtv020sd16p.asyncPlayVoice(random(0, MAX_VOICES));
}

void wakeUp(void)
{
 
}


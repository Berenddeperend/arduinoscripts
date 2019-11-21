#include <Arduino.h>


// const int RECV_PIN = 7;
// const int LED_PIN = 12;

// void setup(){
//   pinMode(12, OUTPUT);
//   Serial.begin(9600);
// }

// void loop(){
// }


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
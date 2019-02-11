#include <Arduino.h>

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D8, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println("Initializing");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(D8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(D8, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  Serial.println(analogRead(A0));

}
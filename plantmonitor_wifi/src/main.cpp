#include <Arduino.h>
#include <IRremote.h>


const int RECV_PIN = 7;
const int LED_PIN = 12;
// IRrecv irrecv(RECV_PIN);
// decode_results results;

void setup(){
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  // irrecv.enableIRIn();
  // irrecv.blink13(true);
}

void loop(){
  // if (irrecv.decode(&results)){
  //       Serial.println(results.value, HEX);
  //       irrecv.resume();
  //       digitalWrite(LED_PIN, HIGH);
  //       delay(100);
  // } else {
  //   digitalWrite(LED_PIN, LOW);
  // }
}

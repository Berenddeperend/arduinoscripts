#include <VirtualWire.h>

const int RF_TX_PIN = 3;

const int switchPin = 12;
const int switchPinA = 11;
const int switchPinB = 12;

const int joystickBtnPin = 9;
const int joystickXPin = A0;
const int joystickYPin = A1;

const int servoXPin = 3;
const int servoYPin = 6;

//544 - 2400
const int xMin = 2400;
const int xMax = 544;
const int yMin = 2400;
const int yMax = 544;

unsigned long debounceDelay = 10;
char switchState;


void setup() {
  Serial.begin(19200);
  Serial.setTimeout(10); //Michael says this fixes a one second delay. Sure, we'll leave it in.

  pinMode(switchPin, OUTPUT);
  pinMode(switchPinA, INPUT);
  pinMode(switchPinB, INPUT);

  digitalWrite(switchPin, HIGH);
  
  vw_set_tx_pin(RF_TX_PIN);
  vw_setup(2000);
}

unsigned char buffer[64]; // buffer array for data recieve over serial port
int count = 0;     // counter for buffer array 

void clearBufferArray() {
  for (int i = 0; i < count; i++) {
    buffer[i] = NULL;
  }
}

void transmitMessage(String(input)) {
  const char *msg = input.c_str(); //convert input string to char array
  vw_send((uint8_t *)msg, strlen(msg));
  Serial.print("sending: ");
  Serial.println(msg);
}

String buildPositionString(int xPos, int yPos){
  String mappedX = String(map(xPos, 0, 1023, xMin, xMax));
  String mappedY = String(map(yPos, 0, 1023, yMin, yMax));
  return String("x" + mappedX + "y" + mappedY);
}

void loop() {

  if(digitalRead(switchPinA) == LOW) {
    switchState = "left";
      transmitMessage(
        buildPositionString(
          analogRead(joystickXPin),
          analogRead(joystickYPin)
        )
      );
  } else {
    switchState = "right";
    transmitMessage(
        buildPositionString(
          analogRead(servoXPin),
          analogRead(servoYPin)
        )
      );
  }
  delay(500);
}

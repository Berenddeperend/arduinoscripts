#include <Arduino.h>

// #include "abtoo.h"
#include <abtoo.h>
 int ledPin = 13; // GPIO13
char* ssid = "swennenhuis";
char* password = "jambosana";
int h = LOW;
 
AbetooIno abtoino;
 
void messageReceived(String message) {
  Serial.println(message);
  h = h^1;
  digitalWrite(ledPin, h?HIGH:LOW);
}
 
void setup() {
  Serial.begin(9600);
  delay(10);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  
  String uuid = "arduino";
  String channel = "-V898dmJStk-FIgeLCMbfhQ636307126199562031#970e53896952cb6fe023a0858025509fbc896d63";
  int heartbeat = 1; // minutes
  abtoino.init(uuid, channel, heartbeat, messageReceived);
}
 
String channeltarget = "-uLj2KEuibEGl0o5tfqauDA636307126198501918#0f82d535bd87d54e8ffd84cd518ecd2d3f90eb7e";
long count = 0;
long messages = 0;

void loop() {
  if (count++ == 1000000) {
    count = 0;
    Serial.print("Sending message...");
    String body = "Message from arduino ";
    body = body + (messages++) + "!";
    abtoino.sendMessage(channeltarget, body, "no correlation id");
    delay(500);
  }
  abtoino.abetooloop();
}
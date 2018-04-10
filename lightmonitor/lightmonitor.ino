int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

void setup() { 
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println(
    String(
      map(
        analogRead(A5), 0, 1024, 0, 100
      )
    )  + "%"
  );



  // analogWrite(3, 255);
  // analogWrite(5, 255);
  // delay(100);


  analogWrite(3, brightness);
  analogWrite(5, brightness);

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

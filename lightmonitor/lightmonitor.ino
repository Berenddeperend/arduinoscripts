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

  delay(100);
}

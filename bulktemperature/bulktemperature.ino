int sensorCount = 4;


void setup() {
  Serial.begin(9600);
  
}

void loop() {

  for (int i = 0; i < sensorCount; i ++) {
    int pin = "A" + i;
    
    float temperature = analogRead(pin);

    float voltage = temperature * 5.0;
    voltage /= 1024.0;
    float degs = (voltage - 0.5) * 100;


   Serial.print("Temperature of sensor nr. "); 
   Serial.print(i); 
   Serial.print(" : ");
   Serial.print(degs); 
   Serial.println("c"); 
  }

  Serial.println("-------------"); 
  delay(2000);
}

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int lightValue;
int temperature;
String message;
String lightMessage;
String temperatureMessage;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  lightValue = analogRead(A1);
  temperature = analogRead(A0);

  float voltage = temperature * 5.0;
  voltage /= 1024.0;
  int degs = (voltage - 0.5) * 100;

  delay(5);
  // moistValue = analogRead(A0);
  // mappedMoistValue = map(moistValue, 0, 1024, 100, 0);
  // message = String(mappedMoistValue) + "%";

  // lightMessage = String(analogRead(A1));


  lightMessage = String(map(analogRead(A1), 0, 1024, 0, 100))  + "%";
  // lightMessage = String((((analogRead(A0)/1024) * 5) - 5) * 100))  + "C°";
  temperatureMessage = String(degs) +  "c";

  lcd.clear();
  lcd.print("Light   Temp.");
  lcd.setCursor(0, 1);
  lcd.print(lightMessage);

  lcd.setCursor(8, 1);
  lcd.print(temperatureMessage);

  delay(250);
}

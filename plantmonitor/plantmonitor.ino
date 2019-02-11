// #include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int moistValue1;
int moistValue2;
int moistValue3;
int mappedMoistValue1;

// int delayTime = 5 * 60 * 1000; //minutes
int delayTime = 200;

// String message;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  // lcd.begin(16, 2);
}

void loop() {
  moistValue1 = analogRead(A1);
//  moistValue2 = analogRead(A1);
//  moistValue3 = analogRead(A2);
   mappedMoistValue1 = map(moistValue1, 0, 1024, 100, 0);
  // mappedMoistValue2 = map(moistValue2, 0, 1024, 100, 0);
  // mappedMoistValue3 = map(moistValue3, 0, 1024, 100, 0);

//  Serial.println("moist=" + String(moistValue1) + "&moist=" + String(moistValue2) + "&moist=" + String(moistValue3));
    Serial.print("moist=");
    Serial.println(String(moistValue1));
  // message = String(mappedMoistValue) + "%";

  // lcd.clear();
  // lcd.print("Vochtniveau");
  // lcd.setCursor(0, 1);

  // lcd.print(message);

  delay(delayTime);
}

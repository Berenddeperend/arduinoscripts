// This version includes a potmeter. Whoo


#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);

extern uint8_t BigNumbers[];

MPU6050 accelgyro;

int potPin = A7;
int motorPin = 3;
int potValue;
int mappedGyroVal;

int16_t ax, ay, az;
int16_t gx, gy, gz;

bool blinkState = false;

void setup() {
    Wire.begin();
    Serial.begin(9600);
    pinMode(motorPin, OUTPUT);
    pinMode(potPin, INPUT);

    myOLED.begin();
    myOLED.setFont(BigNumbers);
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // digitalWrite(motorPin, HIGH);
}

void loop() {
    // read raw accel/gyro measurements from device
    if(millis() % 100 == 0) {
        potValue = analogRead(potPin);

        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  //31072 en -31072 is zijn de maximale values die uit de sensor kunnen komen.    

        mappedGyroVal = map(az, -31072, 31072, 1000, 0);

        myOLED.printNumI(mappedGyroVal, CENTER, 20);
        myOLED.printNumI(potValue, CENTER, 60);
        // myOLED.printNumI(potValue, CENTER, 20);
        // Serial.println(analogRead(potPin));
        myOLED.update();
        myOLED.clrScr();

        if(mappedGyroVal < (potValue + 100)  && mappedGyroVal > (potValue - 100)) {
            digitalWrite(motorPin, HIGH);
        } else {
            digitalWrite(motorPin, LOW);
        }

        // Serial.println(az);
    }

}


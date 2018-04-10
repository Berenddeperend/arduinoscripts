#include "Wire.h"

#include "I2Cdev.h"
#include "MPU6050.h"
#include <SevenSeg.h>

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

//Defines the segments A-G: SevenSeg(A, B, C, D, E, F, G);
SevenSeg disp(9, 11, 6, 4, 3, 8, 7); //5 is decimal


const int numOfDigits = 3;
int digitPins[numOfDigits] = {10, 12, 13};



#define LED_PIN 13
bool blinkState = false;

void setup() {
    Wire.begin();
    Serial.begin(9600);
    
    
    disp.setDigitPins(numOfDigits, digitPins);
    disp.setDutyCycle(100); //brightness 0 - 100
    // disp.setRefreshRate(150);
    // disp.setDigitDelay(1);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}

void loop() {

    // read raw accel/gyro measurements from device
    
    
    if(millis() % 100 == 0) {
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  //31072 en -31072 is zijn de maximale values die uit de sensor kunnen komen.    
    }
    disp.write(map(az, -31072, 31072, 100, 999));
    // disp.write(141);


    // blink LED to indicate activity
    // blinkState = !blinkState;
    // digitalWrite(LED_PIN, blinkState);
}


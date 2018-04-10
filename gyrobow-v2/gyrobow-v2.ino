#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);

extern uint8_t BigNumbers[];

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

bool blinkState = false;

void setup() {
    Wire.begin();
    Serial.begin(9600);
    
    myOLED.begin();
    myOLED.setFont(BigNumbers);
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {

    // read raw accel/gyro measurements from device
    if(millis() % 100 == 0) {
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  //31072 en -31072 is zijn de maximale values die uit de sensor kunnen komen.    

        myOLED.printNumI(map(az, -31072, 31072, 1000, 0), CENTER, 20);
        myOLED.update();
        myOLED.clrScr();

        Serial.println(az);
    }

}


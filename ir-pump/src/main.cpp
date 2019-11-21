// #include <Arduino.h>
// #include <IRremote.h>

// const int RECV_PIN = 7;
// const int LED_PIN = 12;
// IRrecv irrecv(RECV_PIN);
// decode_results results;

// void setup(){
//   pinMode(12, OUTPUT);
//   Serial.begin(9600);
//   irrecv.enableIRIn();
//   irrecv.blink13(true);
// }

// void loop(){
//   if (irrecv.decode(&results)){
//         Serial.println(results.value, HEX);
//         irrecv.resume();
//         digitalWrite(LED_PIN, HIGH);
//         delay(300);
//   } else {
//     digitalWrite(LED_PIN, LOW);
//   }
// }







#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


/* u8g2.begin() is required and will sent the setup/init sequence to the display */
void setup(void) {

  u8x8.begin();
  u8x8.setPowerSave(0);
}

/* draw something on the display with the `firstPage()`/`nextPage()` loop*/
void loop(void) {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(3,2,"Vochtniveau"); // 16 chars lang
}
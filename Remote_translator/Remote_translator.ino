#include <RemoteSwitch.h>

/*
* Demo for RF remote switch receiver.
* For details, see RemoteReceiver.h!
*
* This sketch "translates" a Action-remote to a Blokker-remote.
* When the A-On-button of the Action-remote is pressed, the Blokker-devices
* 5, 6 and 7 are switched on. The A-Off-button switches the devices off again.
*
* Connect the transmitter to digital pin 11, and the receiver to digital pin 2.
*/

ActionSwitch actionSwitch(11);

//Prepare the code for switch A (system code 1) on and off, for easy comparision later.
unsigned long actionAOn = actionSwitch.getTelegram(1,'A',true);
unsigned long actionAOff = actionSwitch.getTelegram(1,'A',false);

void setup() {
  //See example Show_received_code for info on this
  for (int i = 0; i <= 31; i++) {
    actionSwitch.sendSignal(i,'A',true);
    delay(500);
    actionSwitch.sendSignal(i,'B',true);
    delay(500);
    actionSwitch.sendSignal(i,'C',true);
    delay(500);
    actionSwitch.sendSignal(i,'D',true);
    delay(500);
    actionSwitch.sendSignal(i,'E',true);
    delay(500);
  }
}

void loop() {
}



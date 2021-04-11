#include <Arduino.h>
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define wifiLedPin      2 // D4 builtIn led
#define wallSwitchPin   5 // D1 Wall Switch
#define statusLedPin    2 // D4 Wall StatusLed
#define out1            4 // D2
#define out2           14 // D5
#define out3           12 // D6
#define out4           13 // D7

#define NODE_NAME "living"


#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light[] = {
  Light("main", out1, false), // main
  Light("dicro", out2, false), // dicro
  Light("bookshelf", out3, false), // bookshelf
  Light("corner", out4, false)  // corner
};

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  light[0].turnOn();

  //connect
  wemosWiFi.connect(NODE_NAME);
  light[0].turnOff();

  //non-critical hardware
  analogWrite(statusLedPin, 50);
 
}
 
void loop() {
  wemosWiFi.update();
  wallSwitch.update();
}

/// Node HI logic ////////////////////////////////
void switchCallback(int clicks) {
  Serial.print(clicks);
  Serial.println("");
  
  switch (clicks)
  {
  case -1:
    turnAllOff();
    break;

  case 1:
    if(light[0].isOn){
      turnAllOff();
    }else{
      light[0].turnOn();
    };
    break;

  case 2:
    light[1].toggle();
    break;

  case 3:
    light[2].toggle();
    break;

  case 4:
    light[3].toggle();
    break;

  default:
    break;
  }
}

void turnAllOff(){
  light[0].turnOff();
  light[1].turnOff();
  light[2].turnOff();
  light[3].turnOff();
}
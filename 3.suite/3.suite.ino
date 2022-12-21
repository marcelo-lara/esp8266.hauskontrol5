#include <Arduino.h>
#include "src/wemos.setup/wemos.setup.h"
#include "src/WebApi/WebApi.h"

/////////////////////////////////////////
// Hardware Setup
#define wifiLedPin      2 // D4 builtIn led
#define wallSwitchPin   5 // D1 Wall Switch
#define statusLedPin    2 // D4 Wall StatusLed

//light
#define lightOut        4 // D2

//fan
#define fanOut1           13 // D7
#define fanOut2           12 // D6
#define fanOut3           14 // D5
#define fanOut4           15 // D8
#define defSpeed           4 // default speed
///////////////////////////////////////////

#define NODE_NAME "suite"
WebApi api(WebApi::Controller::Suite);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(lightOut, true); // main
void handleLight(String *args);

#include "src/Core/Devices/Fan/Fan.h"
Fan fan(true, fanOut1, fanOut2, fanOut3, fanOut4, defSpeed); // suite fan

Device* devices[] = {&light, &fan};


void setup() {

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  light.turnOn();

  //connect
  wemosWiFi.connect(NODE_NAME);
  
  //link devices
  api.set_devices(devices, 2);
  api.setup();
 
  //mqtt publish
  light.statusChanged=[](String topic, bool state){api.mqtt_publish(&light);};
  fan.statusChanged=[](int outSpeed){api.mqtt_publish((Fan*)(&fan));};

  //non-critical hardware
  pinMode(statusLedPin, OUTPUT);
  analogWrite(statusLedPin, 50);
 
  light.turnOff();

}
 
void loop() {
  wallSwitch.update();
  wemosWiFi.update();
  api.update();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void switchCallback(int clicks) {

  switch (clicks)
  {
  case BTN_CLICK:
    light.toggle();
    break;
  
  case BTN_DBLCLICK:
    fan.toggle();
    break;

  case BTN_LONGCLICK:
    light.turnOff();
    fan.turnOff();
    break;

  default:
    break;
  }
    
}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "src/wemos.setup/wemos.setup.h"
#include "src/WebApi/WebApi.h"

/////////////////////////////////////////
// Hardware Setup

//fan
#define out1           13 // D7
#define out2           12 // D6
#define out3           14 // D5
#define out4           15 // D8

//fan
#include "src/Core/Devices/Fan/Fan.h"
Fan fan(true, out1, out2, out3, out4, defSpeed, true); // 

///////////////////////////////////////////

#define NODE_NAME "smartplug"
WebApi api(WebApi::Controller::SmartPlug);
Device* devices[] = {&fan};

void setup() {

  //connect
  wemosWiFi.connect(NODE_NAME);
  
  //link devices
  api.set_devices(devices, 1);
  api.setup();
 
  //mqtt publish
  fan.statusChanged=[](int outSpeed){api.mqtt_publish((Fan*)(&fan));};


}
 
void loop() {
  wemosWiFi.update();
  api.update();
}

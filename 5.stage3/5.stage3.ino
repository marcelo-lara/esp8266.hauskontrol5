#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "src/wemos.setup/wemos.setup.h"
#include "src/WebApi/WebApi.h"

/////////////////////////////////////////
// Hardware Setup

//fan
#define fanOut1           13 // D7
#define fanOut2           12 // D6
#define fanOut3           14 // D5
#define fanOut4           15 // D8
#define defSpeed           3 // default speed

//fan
#include "src/Core/Devices/Fan/Fan.h"
Fan fan(true, fanOut1, fanOut2, fanOut3, fanOut4, defSpeed); // stage3 fan

///////////////////////////////////////////

#define NODE_NAME "stage3"
WebApi api(WebApi::Controller::Stage3);
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

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "src/Core/Web/WebUi.h"
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define fan_light 5

//shifted out
#include "src/Core/Io/ShiftedIo/ShiftedIo.h"
#define latchPin 13 // D7 violet
#define dataPin  12 // D6 blue
#define clockPin 14 // D5 green
ShiftedIo shiftedIo(latchPin, dataPin, clockPin, true);

//fan
#include "src/Core/Devices/Fan/Fan.h"
#define defSpeed           3 // default speed
Fan fan(&shiftedIo, defSpeed); // suite fan

//light
Light light(&shiftedIo, fan_light);

///////////////////////////////////////////

#define NODE_NAME "stage3"
WebUi ui(Controller::Stage3, NODE_NAME);

void setup() {
  light.turnOn();

  //connect
  wemosWiFi.connect(NODE_NAME);

  // web ui
  ui.add_device(&fan);
  ui.add_device(&light);
  ui.init();

  light.turnOff();

}
 
void loop() {
  wemosWiFi.update();
  ui.update();
}

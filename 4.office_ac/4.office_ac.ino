#include <Arduino.h>
#include "src/WebApi/WebApi.h"
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define irSendPin      14 // D5 infrared signal out
#define irRecvPin      13 // D7 infrared signal out

#define wifiLedPin      2 // D4 builtIn led

#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)

#define acOnSwitchPin  15 // D8 pullup 
#define acOnLedPin     12 // D6
///////////////////////////////////////////

#define NODE_NAME "officeac"
WebApi api(WebApi::Controller::OfficeAc);

//// devices ////
#include "src/Core/Devices/AC/AC.h"
AC ac(irSendPin, 24);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;
Device* devices[] = {&ac, &environment};

void setup() {

  //core devices
  ac.init();

  //connect
  wemosWiFi.connect(NODE_NAME);

  //non-critical hardware
  // environment.setup();
  // environment.statusUpdated = [](bool state){api.mqtt_publish(&environment);};

  //web ui
  api.set_devices(devices, 2);
  api.setup();

  pinMode(acOnLedPin, OUTPUT);
  digitalWrite(acOnLedPin, HIGH);
}

void loop() {
  wemosWiFi.update();
  api.update();

  // environment.update();

}

#include <Arduino.h>
#include "src/wemos.setup/wemos.setup.h"
#include "src/WebApi/WebApi.h"

/////////////////////////////////////////
// Hardware Setup
#define wallSwitchPin  14 // D5 Wall Switch
#define statusLedPin   12 // D6 Wall StatusLed
#define relayOutPin    13 // D7 relay out
#define irSendPin       0 // D3 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led

#define NODE_NAME "office"
WebApi api(WebApi::Controller::Office);

//// devices ////
#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(relayOutPin, true);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;

Device* devices[] = {&light, &environment};


void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);
  light.turnOn();

  wemosWiFi.connect(NODE_NAME);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=[](int cc) {light.toggle();};

  //connect
  api.set_devices(devices, 2);
  api.setup();
  light.statusChanged=[](String topic, bool state){api.mqtt_publish(&light);};

  //non-critical hardware
  environment.setup();
  environment.statusUpdated = [](bool state){api.mqtt_publish(&environment);};

  light.turnOff();
  digitalWrite(statusLedPin, HIGH);

};
 
void loop() {
  wemosWiFi.update();
  api.update();

  wallSwitch.update();
  environment.update();
};

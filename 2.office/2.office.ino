#include <Arduino.h>
#include "src/wemos.setup/wemos.setup.h"
#include "src/Core/Comm/MqttClient.h"
#include "src/Core/Web/WebUi.h"

/////////////////////////////////////////
// Hardware Setup
#define wallSwitchPin  14 // D5 Wall Switch
#define statusLedPin   12 // D6 Wall StatusLed
#define relayOutPin    13 // D7 relay out
#define irSendPin       0 // D3 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led

#define NODE_NAME "dummyoffice"
WebUi ui(Controller::Office, NODE_NAME);
MqttClient mqtt_client;

//// devices ////
#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(relayOutPin, true);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;



void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=[](int cc) {light.toggle();};
  light.turnOn();

  //connect
  wemosWiFi.connect(NODE_NAME);
  mqtt_client.setup();

  // Start the server
  ui.add_device(&light);
  ui.add_device(&environment);
  ui.init();
 
  //non-critical hardware
  light.turnOff();
  environment.setup();
  digitalWrite(statusLedPin, HIGH);

}
 
void loop() {
  wallSwitch.update();
  environment.update();

  wemosWiFi.update();
  mqtt_client.update();
  ui.update();

}


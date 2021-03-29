#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <uri/UriRegex.h>
#include "src/Core/Web/WebUi.h"
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define irSendPin      14 // D5 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led

#define acOnSwitchPin  12 // D6 pullup 
#define acOnLedPin     13 // D7
///////////////////////////////////////////

#define NODE_NAME "officeac"
WebUi ui(NODE_NAME);

//// devices ////
#include "src/Core/Devices/AC/AC.h"
AC ac(irSendPin, 24);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;

void setup() {

  //core devices
  ac.init();

  //connect
  wemosWiFi.connect(NODE_NAME);

  //non-critical hardware
  environment.setup();

  //web ui
  ui.add_device(&ac);
  ui.add_device(&environment);
  ui.init();


  pinMode(acOnLedPin, OUTPUT);
  digitalWrite(acOnLedPin, HIGH);
}

void loop() {
  wemosWiFi.update();

  environment.update();
  ui.update();

}

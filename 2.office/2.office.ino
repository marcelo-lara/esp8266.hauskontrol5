#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <uri/UriRegex.h>
#include "src/Core/Web/WebServerHelper.h"
#include "src/wemos.setup/wemos.setup.h"

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
ESP8266WebServer server(80);    // WebServer object
WebServerHelper srv(&server);

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
  light.turnOff();

  // Start the server
  server_setup();
 
  //non-critical hardware
  environment.setup();
  digitalWrite(statusLedPin, HIGH);

}
 
void loop() {
  wallSwitch.update();
  environment.update();

  //
  wemosWiFi.update();
  server.handleClient();

}

void server_setup(){
  // controller
  server.on("/",                []() { return ui_root();    });
  server.on("/status",          []() { return status_json();});

  // light
  srv.add_light(&light);

  //server
  srv.init();
}

void ui_root(){
  String dev_html;
  dev_html += "<h2>lights</h2>";
  dev_html += light.to_html_div();
  dev_html += "<h2>environment</h2>";
  dev_html += environment.to_html_div();

  return srv.send_root(dev_html, NODE_NAME);
}

void status_json(){
  String json_dev_list;
  json_dev_list += "{";
  json_dev_list += srv.json_obj_block("lights", light.to_json());
  json_dev_list += ",";
  json_dev_list += srv.json_obj_block("env", environment.to_json());
  json_dev_list += "}";
  srv.send_status(json_dev_list);
}


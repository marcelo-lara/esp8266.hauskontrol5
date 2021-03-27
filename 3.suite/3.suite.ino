#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <uri/UriRegex.h>
#include "src/Core/Web/WebServerHelper.h"
#include "src/wemos.setup/wemos.setup.h"

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
#define defSpeed           3 // default speed
///////////////////////////////////////////


#define NODE_NAME "suite"
#include <ESP8266WebServer.h> // web server
ESP8266WebServer server(80);    
WebServerHelper srv(&server);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(lightOut, true); // main
void handleLight(String *args);

#include "src/Core/Devices/Fan/Fan.h"
Fan fan(fanOut1, fanOut2, fanOut3, fanOut4, defSpeed); // suite fan

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  light.turnOn();

  //connect
  wemosWiFi.connect("suite");
  light.turnOff();

  // Start the server
  server_setup();
 
  //non-critical hardware
  analogWrite(statusLedPin, 50);

 
}
 
void loop() {
  wemosWiFi.update();

  wallSwitch.update();
  server.handleClient();

 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HTTP_OK  server.send(200, "text/plain", "OK")
#define HTTP_404 server.send(404, "text/plain", "404: Not found")
void server_setup(){

  //core
  server.on("/",                  []() { return ui_root(); });
  server.on("/status",            []() { return status_json(); });

  //fan
  server.on("/set/fan/on",        []() { fan.turnOn();    return HTTP_OK; });
  server.on("/set/fan/off",       []() { fan.turnOff();   return HTTP_OK; });
  server.on("/set/fan/speed/1",   []() { fan.setSpeed(1); return HTTP_OK; });
  server.on("/set/fan/speed/2",   []() { fan.setSpeed(2); return HTTP_OK; });
  server.on("/set/fan/speed/3",   []() { fan.setSpeed(3); return HTTP_OK; });
  server.on("/set/fan/speed/4",   []() { fan.setSpeed(4); return HTTP_OK; });

  // light
  srv.add_light(&light);

  //server
  srv.init();
}

void ui_root(){
  String dev_html;
  dev_html += "<h2>lights</h2>";
  dev_html += light.to_html_div();
  dev_html += "<h2>fan</h2>";
  dev_html += fan.to_html();

  return srv.send_root(dev_html, NODE_NAME);
}

void status_json(){
  String json_dev_list;
  json_dev_list += srv.json_obj_block("lights", light.to_json()) + ",";
  json_dev_list += srv.json_obj_block("fan", fan.to_json());
  srv.send_status(json_dev_list);
}


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

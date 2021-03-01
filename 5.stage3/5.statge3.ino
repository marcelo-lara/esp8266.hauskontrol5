#include <Arduino.h>
#include <ESP8266WiFi.h>
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
///////////////////////////////////////////

#include <ESP8266WebServer.h> // web server
ESP8266WebServer server(80);    

void setup() {
  shiftedIo.setIo(fan_light, true);

  //connect
  wemosWiFi.connect("stage3");

  // web ui
  server_setup();
  shiftedIo.setIo(fan_light, false);

}
 
void loop() {
  wemosWiFi.update();
  server.handleClient();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HTTP_OK  server.send(200, "text/plain", "OK")
#define HTTP_404 server.send(404, "text/plain", "404: Not found")
void server_setup(){

  //core
  server.on("/",                  []() { server.send(200, "text/html", ui_root()); return; });
  server.on("/status",            []() { server.send(200, "application/json", status_pattern(fan.isOn)); return; });

  //fan
  server.on("/get/fan",           []() { return server.send(200, "application/json", status_pattern(fan.isOn));});
  server.on("/set/fan/on",        []() { fan.turnOn();    return HTTP_OK; });
  server.on("/set/fan/off",       []() { fan.turnOff();   return HTTP_OK; });
  server.on("/set/fan/speed/1",   []() { fan.setSpeed(1); return HTTP_OK; });
  server.on("/set/fan/speed/2",   []() { fan.setSpeed(2); return HTTP_OK; });
  server.on("/set/fan/speed/3",   []() { fan.setSpeed(3); return HTTP_OK; });
  server.on("/set/fan/speed/4",   []() { fan.setSpeed(4); return HTTP_OK; });
  server.on("/set/fan/speed/5",   []() { fan.setSpeed(5); return HTTP_OK; });

  //light
  server.on("/get/light",         []() { return server.send(200, "application/json", status_pattern(shiftedIo.getIo(fan_light))); });
  server.on("/set/light/on",      []() { shiftedIo.setIo(fan_light, true); return HTTP_OK; });
  server.on("/set/light/off",     []() { shiftedIo.setIo(fan_light, false); return HTTP_OK; });

  //undhandleds
  server.onNotFound([](){HTTP_404;});  
  server.begin();

}

String ui_root(){
  String r_body;
  r_body += "<html>";
  r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title></title></head><body>";
  r_body += ("</body></html>");
  return r_body;
}

String status_pattern(bool state){
  String r_body;
  r_body += "{\"statusPattern\":\"";
  r_body += state?"true":"false";
  r_body += "\"}";
  return r_body;
}

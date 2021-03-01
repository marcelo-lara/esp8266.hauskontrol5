#include <Arduino.h>
#include <ESP8266WiFi.h>
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

#include <ESP8266WebServer.h> // web server
ESP8266WebServer server(80);    

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

  //light
  server.on("/get/light",         []() { return server.send(200, "application/json", status_pattern(light.isOn)); });
  server.on("/set/light/on",      []() { light.turnOn();  return HTTP_OK; });
  server.on("/set/light/off",     []() { light.turnOff(); return HTTP_OK; });
  server.on("/set/light/toggle",  []() { light.toggle();  return HTTP_OK; });

  //undhandleds
  server.onNotFound([](){HTTP_404;});  
  server.begin();
}

String ui_root(){
  String r_body;
  r_body += "<html>";
  r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title></title></head><body>";
  r_body += "<h1>Suite</h1>";
 
  r_body += "<h2>light</h2>";
  r_body += "<span>";
  r_body += light.isOn?"On":"Off";
  r_body += "</span>";
  r_body += "<a href=\"/set/light/toggle\" target=\"i_result\"><button>switch</button></a><br>";

  r_body += "<h2>fan</h2>";
  r_body += "<span>";
  r_body += fan.isOn?"On":"Off";
  r_body += "</span>";
  r_body += "<a href=\"/set/fan/off\" target=\"i_result\"><button>off</button></a>\
    <a href=\"/set/fan/speed/1\" target=\"i_result\"><button>1</button></a>\
    <a href=\"/set/fan/speed/2\" target=\"i_result\"><button>2</button></a>\
    <a href=\"/set/fan/speed/3\" target=\"i_result\"><button>3</button></a>\
    <a href=\"/set/fan/speed/4\" target=\"i_result\"><button>4</button></a>";
  r_body += "<iframe name=\"i_result\" height=\"20px\" width=\"100%\" style=\"display:none\"></iframe>";

  r_body += "</body></html>";
  return r_body;
}

String status_pattern(bool state){
  String r_body;
  r_body += "{\"statusPattern\":\"";
  r_body += state?"true":"false";
  r_body += "\"}";
  return r_body;
}

/////////////////////////////////////////////////////////////

void handleStatus(String *args, WiFiClient *client){
  client->println("HTTP/1.1 200 OK");
  client->println("Content-Type: application/json");
  client->println(""); //  do not forget this one

  //light
  if (args->indexOf("/status/light/1") != -1)  {
    client->print("{\"statusPattern\": \""); 
    client->print(light.isOn?"true":"false"); 
    client->println("\"}");
    return;
  }

  //fan
  if (args->indexOf("/status/fan") != -1)  {
    client->print("{\"statusPattern\": \""); 
    client->print(fan.isOn?"true":"false"); 
    // client->print("{\"speed\": \""); 
    // client->print(fan.speed); 
    client->println("\"}");
    return;
  }

}

void handleLight(String *args){
  if (args->indexOf("/set/light/1/on") != -1)     light.turnOn();
  if (args->indexOf("/set/light/1/off") != -1)    light.turnOff();
  if (args->indexOf("/set/light/1/toggle") != -1) light.toggle();
};

void handleFan(String *args){
  if (args->indexOf("/set/fan/on") != -1)      fan.turnOn();
  if (args->indexOf("/set/fan/off") != -1)     fan.turnOff();
  if (args->indexOf("/set/fan/toggle") != -1)  fan.toggle();
  if (args->indexOf("/set/fan/speed/1") != -1) fan.setSpeed(1);
  if (args->indexOf("/set/fan/speed/2") != -1) fan.setSpeed(2);
  if (args->indexOf("/set/fan/speed/3") != -1) fan.setSpeed(3);
  if (args->indexOf("/set/fan/speed/4") != -1) fan.setSpeed(4);
};


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

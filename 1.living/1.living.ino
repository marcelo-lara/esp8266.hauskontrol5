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
#define out1            4 // D2
#define out2           14 // D5
#define out3           12 // D6
#define out4           13 // D7

ESP8266WebServer server(80);    // WebServer object
WebServerHelper srv(&server);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light[] = {
  Light(out1, false), // main
  Light(out2, false), // dicro
  Light(out3, false), // bookshelf
  Light(out4, false)  // corner
};

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  light[0].turnOn();

  //connect
  wemosWiFi.connect("dummy");
  light[0].turnOff();

  // WebServer
  server_setup();
 
  //non-critical hardware
  analogWrite(statusLedPin, 50);
 
}
 
void loop() {
  wemosWiFi.update();
  server.handleClient();
  wallSwitch.update();
}

void server_setup(){
  // node
  server.on("/",                []() { return ui_root();});
  server.on("/status",          []() { return status_json();});

  // siri response
  server.on("/pattern/light/1", []() { return srv.status_pattern(light[0].isOn); });
  server.on("/pattern/light/2", []() { return srv.status_pattern(light[1].isOn); });
  server.on("/pattern/light/3", []() { return srv.status_pattern(light[2].isOn); });
  server.on("/pattern/light/4", []() { return srv.status_pattern(light[3].isOn); });

  // lights
  server.on("/set/light/1/on",     []() { light[0].turnOn();  return srv.send_result("ok"); });
  server.on("/set/light/1/off",    []() { light[0].turnOff(); return srv.send_result("ok"); });
  server.on("/set/light/1/toggle", []() { light[0].toggle();  return srv.send_result("ok"); });
  server.on("/set/light/2/on",     []() { light[1].turnOn();  return srv.send_result("ok"); });
  server.on("/set/light/2/off",    []() { light[1].turnOff(); return srv.send_result("ok"); });
  server.on("/set/light/2/toggle", []() { light[1].toggle();  return srv.send_result("ok"); });
  server.on("/set/light/3/on",     []() { light[2].turnOn();  return srv.send_result("ok"); });
  server.on("/set/light/3/off",    []() { light[2].turnOff(); return srv.send_result("ok"); });
  server.on("/set/light/3/toggle", []() { light[2].toggle();  return srv.send_result("ok"); });
  server.on("/set/light/4/on",     []() { light[3].turnOn();  return srv.send_result("ok"); });
  server.on("/set/light/4/off",    []() { light[3].turnOff(); return srv.send_result("ok"); });
  server.on("/set/light/4/toggle", []() { light[3].toggle();  return srv.send_result("ok"); });

  //server
  srv.init();

  // Print IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}


void ui_root(){
  String dev_html;
  dev_html += "<h2>light</h2>";

  //lights
  for (int i = 0; i < 4; i++){
    dev_html += "<div";
    dev_html += " class=\"button " + String(light[i].isOn?"on":"off") + "\"";
    dev_html += " target=\"set/light/" + String(i+1) + "/toggle\"";
    dev_html += ">";
    dev_html += "light "+ String(i+1);
    dev_html += "</div>";
  }

  return srv.send_root(dev_html);
}

void status_json(){
  String json_dev_list;
  for (int i = 0; i < 4; i++)
    json_dev_list += light[i].to_json() + String( i<3?",":"");
  
  srv.send_status(json_dev_list);
}

void switchCallback(int clicks) {
  Serial.print(clicks);
  Serial.println("");
  
  switch (clicks)
  {
  case -1:
    turnAllOff();
    break;

  case 1:
    if(light[0].isOn){
      turnAllOff();
    }else{
      light[0].turnOn();
    };
    break;

  case 2:
    light[1].toggle();
    break;

  case 3:
    light[2].toggle();
    break;

  case 4:
    light[3].toggle();
    break;

  default:
    break;
  }
}

void turnAllOff(){
  light[0].turnOff();
  light[1].turnOff();
  light[2].turnOff();
  light[3].turnOff();
}
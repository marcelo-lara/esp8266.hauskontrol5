#include <Arduino.h>
#include "src/wemos.setup/wemos.setup.h"
#include <ESP8266WiFi.h>
#include "src/WebApi/WebApi.h"

/////////////////////////////////////////
// Hardware Setup
#define wifiLedPin      2 // D4 builtIn led
#define wallSwitchPin   5 // D1 Wall Switch
#define statusLedPin    2 // D4 Wall StatusLed
#define out1            4 // D2
#define out2           14 // D5
#define out3           12 // D6
#define out4           13 // D7

#define NODE_NAME "living"
WebApi api(WebApi::Controller::Living);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);
const char* wallSwitch_topic = "hauskontrol/living/button/cmd";
const char* wallSwitch_cmd_on  = "ON";
const char* wallSwitch_cmd_off = "OFF";

#include "src/Core/Devices/Light/Light.h"
Light l_main = Light("main", out1, false); // main
Light l_dicro = Light("dicro", out2, false); // dicro
Light l_bookshelf = Light("bookshelf", out3, false); // bookshelf
Light l_corner = Light("corner", out4, false);  // corner
Device* light[] = {&l_main, &l_dicro, &l_bookshelf, &l_corner};

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  l_corner.turnOn();

  //connect
  wemosWiFi.connect(NODE_NAME);

  //link devices
  api.set_devices(light, 4);
  api.setup();

  l_main.statusChanged = light_callback;
  l_dicro.statusChanged = light_callback;
  l_bookshelf.statusChanged = light_callback;
  l_corner.statusChanged = light_callback;
  // reinterpret_cast<Light*>(light[0])->statusChanged=light_callback;

  //non-critical hardware
  analogWrite(statusLedPin, 50);
  l_corner.turnOff();
}
 
void loop() {
  wemosWiFi.update();
  wallSwitch.update();
  api.update();
}

void light_callback(String topic, bool state){
  Serial.println(topic);
  api.mqtt_publish(topic.c_str(), state?"ON":"OFF");
};

/// Node HI logic ////////////////////////////////
void switchCallback(int clicks) {
  Serial.print(clicks);
  Serial.println("");
  
  switch (clicks){
  case -1:
    turnAllOff();
    break;

  case 1:
    api.mqtt_publish(wallSwitch_topic, wallSwitch_cmd_on);

    if(l_main.isOn){
      turnAllOff();
    }else{
      l_main.turnOn();
    };

    api.mqtt_publish(wallSwitch_topic, wallSwitch_cmd_off);
    break;

  case 2:
    l_dicro.toggle();
    break;

  case 3:
    l_bookshelf.toggle();
    break;

  case 4:
    l_corner.toggle();
    break;

  default:
    break;
  }
}

void turnAllOff(){
  l_dicro.turnOff();
  l_corner.turnOff();
  l_bookshelf.turnOff();
  api.mqtt_publish(wallSwitch_topic, wallSwitch_cmd_off);
}
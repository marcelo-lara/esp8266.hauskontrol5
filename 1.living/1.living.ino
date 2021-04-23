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
const char* wallSwitch_topic = "hauskontrol/living/button";
const char* wallSwitch_cmd_on  = "ON";
const char* wallSwitch_cmd_off  = "OFF";

#include "src/Core/Devices/Light/Light.h"
Light l_main = Light("main"); // main
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

  l_dicro.statusChanged = [](String topic, bool state){api.mqtt_publish(&l_dicro);};
  l_bookshelf.statusChanged = [](String topic, bool state){api.mqtt_publish(&l_bookshelf);};
  l_corner.statusChanged = [](String topic, bool state){api.mqtt_publish(&l_corner);};
  l_main.statusChanged = [](String topic, bool state){
    api.mqtt_publish(wallSwitch_topic, state?wallSwitch_cmd_on:wallSwitch_cmd_off);
  };

  api.mqttTopicReceivedCb = handle_mqtt_callback;

  //non-critical hardware
  analogWrite(statusLedPin, 50);
  l_corner.turnOff();
}
 
void loop() {
  wemosWiFi.update();
  wallSwitch.update();
  api.update();
}

// mqtt callback
void handle_mqtt_callback(String _topic, String _payload){
  Serial.printf("mqtt | [%s] %s\n", _topic.c_str(), _payload.c_str());

  if(_topic.equals("hauskontrol/living/light/main")){
    l_main.isOn=_payload.equals("on");
  };

};

/// Node HI logic ////////////////////////////////
void switchCallback(int clicks) {
  Serial.printf("button | %i\n", clicks);
  
  switch (clicks){
  case -1:
    turnAllOff();
    break;

  case 1:
    l_main.toggle();
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
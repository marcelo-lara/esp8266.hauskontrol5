#include "WebApi.h"
#include <ESP8266WiFi.h>
WiFiClient wifiClient;

WebApi::WebApi(Controller _controller){
    this->server = new ESP8266WebServer(80);
    this->mqtt = new PubSubClient(wifiClient);

    this->controller=_controller;
    switch (this->controller){
        case Living: this->node_name="Living";break;
        case Office: this->node_name="Office";break;
        case OfficeAc: this->node_name="OfficeAc";break;
        case Stage3: this->node_name="Stage 3";break;
    }
};

void WebApi::set_devices(Device** _dev_list, int _dev_count){
    this->device_list = _dev_list;
    this->device_count = _dev_count;
};

void WebApi::setup(){
    this->setup_web();
    this->setup_mqtt();
};

void WebApi::update(){
    this->server->handleClient();

  if (!this->mqtt->connected()) {this->mqtt_connect();};
  this->mqtt->loop();


};


////////////////////////
// MQTT
#define MQTT_CLIENT_ID "office_light1"
#define MQTT_SERVER_IP "192.168.1.201"
#define MQTT_SERVER_PORT 1883
#define MQTT_USER "darkblue"
#define MQTT_PASSWORD "mju76yhn"

void WebApi::setup_mqtt(){
  this->mqtt->setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);

//WORKS OK (no 'this' access)
  this->mqtt->setCallback([&](char *topic, byte *payload, unsigned int length) {  
    //this->mqtt_publish(); 
    Serial.print("| ");
    // Serial.print(node_name);
    Serial.print(" | Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  });

// DOES NOT WORK
  // this->mqtt->setCallback([this](char *topic, byte *payload, unsigned int length) {  
  //   //this->mqtt_publish(); 
  //   Serial.print("| ");
  //   // Serial.print(node_name);
  //   Serial.print(" | Message arrived [");
  //   Serial.print(topic);
  //   Serial.print("] ");
  //   for (int i=0;i<length;i++) {
  //     Serial.print((char)payload[i]);
  //   }
  //   Serial.println();
  // });

// DOES NOT WORK
  // this->mqtt->setCallback(this->mqtt_callback);
  // this->mqtt->setCallback(((void*)this->mqtt_callback));


};

void WebApi::mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("| ");
  Serial.print(this->node_name);
  Serial.print(" | Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
};

void WebApi::mqtt_publish() {
  this->mqtt->publish("hauskontrol/office/light/main/get", "ON", true);
};

void WebApi::mqtt_connect() {
  // Loop until we're reconnected
  while (!this->mqtt->connected()) {
    Serial.println("MQTT| connection...");

    if (this->mqtt->connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("MQTT| connected");
      this->mqtt->subscribe("hauskontrol/office/light/main/set");
      this->mqtt_publish();
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(this->mqtt->state());
      Serial.println("DEBUG: try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000); 
    }
  }
};

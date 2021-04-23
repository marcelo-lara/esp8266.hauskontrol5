#include "WebApi.h"
////////////////////////
// MQTT blocks

#define MQTT_CLIENT_ID "living_controller"
#define MQTT_SERVER_IP "192.168.1.201"
#define MQTT_SERVER_PORT 1883
#define MQTT_USER "darkblue"
#define MQTT_PASSWORD "mju76yhn"
WiFiClient wifiClient;

// Topic definitions
static const char* mqtt_root = "hauskontrol";

void WebApi::setup_mqtt(){
  this->mqtt = new PubSubClient(wifiClient);
  this->mqtt->setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  this->mqtt->setCallback([this] (char* topic, byte* payload, unsigned int length) { this->mqtt_callback(topic, payload, length); });

  String ctrl_root = String(String(mqtt_root) + "/" + String(this->node_name) + "/");

  for (int i = 0; i < this->device_count; i++) {
    switch (this->device_list[i]->type){
      case Device::DevType_e::Light:
        this->device_list[i]->topic = String(ctrl_root + "light/" + this->device_list[i]->name).c_str();
        this->device_list[i]->topic_listen = String(ctrl_root + "light/" + this->device_list[i]->name + "/set").c_str();
        break;
    
    default:
        this->device_list[i]->topic = String(ctrl_root + this->device_list[i]->name).c_str();
        break;
    }
  };
  
};

void WebApi::mqtt_connect() {
  
  if (this->mqtt->connected()) return;
  if (this->mqtt_retry_time > millis()) return;

  Serial.print("mqtt | connecting... ");

    //const char* client_id = String(this->node_name + "_controller").c_str();

    if (this->mqtt->connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("ok!");

      for (int i = 0; i < this->device_count; i++) {
        
        if(this->device_list[i]->isVirtual){

          // virtual device listen to state updates
          this->mqtt->subscribe(String(this->device_list[i]->topic).c_str());
        }else{

          // actual device listen to setter topic and publish updates
          this->mqtt->subscribe(String(this->device_list[i]->topic + "/set").c_str());
          this->mqtt_publish(this->device_list[i]);
        };

      };

    } else {
      Serial.print("FAILED!! state:");
      Serial.println(this->mqtt->state());

      this->mqtt_retry_time = millis() + 5000; //wait 5s to reconnect
    }
};

void WebApi::mqtt_subscribe(const char* topic){
  this->mqtt->subscribe(topic);
};

void WebApi::mqtt_publish(const char* _topic, const char* _message){
  Serial.printf("mqtt publish | %s: %s", _topic, _message);
  this->mqtt->publish(_topic, _message, true);
};

void WebApi::mqtt_publish(Device* dev){
  const char* topic = dev->topic.c_str();
  const char* msg = dev->isOn?"ON":"OFF";

  switch (dev->type){

    // case Device::DevType_e::Light:

    //   break;

    default:

      this->mqtt->publish(dev->topic.c_str(), dev->isOn?"ON":"OFF", true);
      break;

  };

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback commands

void WebApi::mqtt_callback(char* topic, byte* p_payload, unsigned int length) {
  
  // get payload
  String payload;
  for (uint8_t i = 0; i < length; i++) {payload.concat((char)p_payload[i]);};

  // run commands
  for (int i = 0; i < this->device_count; i++) {
    if (!this->device_list[i]->topic_listen.equals(topic)) continue;
    switch (this->device_list[i]->type){

    case Device::DevType_e::Light:
      if (payload.equals(String("ON"))){ this->device_list[i]->turnOn(); };
      if (payload.equals(String("OFF"))){ this->device_list[i]->turnOff(); };
      this->mqtt_publish(this->device_list[i]);
      break;
    
    };
  };

  // trigger callback
  if(this->mqttTopicReceivedCb != nullptr) this->mqttTopicReceivedCb(topic, payload);

};

#include "WebApi.h"
////////////////////////
// MQTT blocks

#define MQTT_CLIENT_ID "suite_controller"
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

      case Device::DevType_e::Light:{
        this->device_list[i]->topic = String(ctrl_root + "light/" + this->device_list[i]->name).c_str();
        break;
      }

      case Device::DevType_e::Fan:{
        ((Fan*)this->device_list[i])->topic = String(ctrl_root + "fan");
        ((Fan*)this->device_list[i])->topic_status = String(ctrl_root + "fan/status");
        ((Fan*)this->device_list[i])->topic_speed  = String(ctrl_root + "fan/speed");
        ((Fan*)this->device_list[i])->topic_mode   = String(ctrl_root + "fan/mode");

        ((Fan*)this->device_list[i])->topic_status_set = String(ctrl_root + "fan/status/set");
        ((Fan*)this->device_list[i])->topic_speed_set = String(ctrl_root + "fan/speed/set");
        ((Fan*)this->device_list[i])->topic_mode_set  = String(ctrl_root + "fan/mode/set");

        break;
      }
    
      default:{
          this->device_list[i]->topic = String(ctrl_root + this->device_list[i]->name).c_str();
          break;
      }

    };
  };
  
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebApi::mqtt_connect() {
  
  if (this->mqtt->connected()) return;
  if (this->mqtt_retry_time > millis()) return;

  Serial.print("mqtt | connecting... ");

    //const char* client_id = String(this->node_name + "_controller").c_str();

    if (this->mqtt->connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("ok!");

      for (int i = 0; i < this->device_count; i++) {
        switch (this->device_list[i]->type){

          case Device::DevType_e::Light:{
              if(this->device_list[i]->isVirtual){

                // virtual device only listen to state updates
                this->mqtt_subscribe(String(this->device_list[i]->topic).c_str());
              }else{

                // actual device listen to setter topic and publish updates
                this->mqtt_subscribe(String(this->device_list[i]->topic + "/set").c_str());
                this->mqtt_publish(this->device_list[i]);
              };
            break;
          }

          case Device::DevType_e::Fan:{
            Fan* fan = (Fan*)this->device_list[i];
            this->mqtt_subscribe(fan->topic_status_set.c_str());
            this->mqtt_subscribe(fan->topic_speed_set.c_str());
            this->mqtt_subscribe(fan->topic_mode_set.c_str());
            this->mqtt_publish(fan);
            break;
          }

        };
        

      };

    } else {
      Serial.print("FAILED!! state:");
      Serial.println(this->mqtt->state());

      this->mqtt_retry_time = millis() + 5000; //wait 5s to reconnect
    }
};

void WebApi::mqtt_subscribe(const char* topic){
  Serial.print("mqtt subscribe| ");
  Serial.println(topic);

  this->mqtt->subscribe(topic);
};

// Callback commands

void WebApi::mqtt_callback(char* topic, byte* p_payload, unsigned int length) {
  
  // get payload
  String payload;
  for (uint8_t i = 0; i < length; i++) {payload.concat((char)p_payload[i]);};

  //debug
  Serial.print("mqtt callback| ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);

  // run commands
  for (int i = 0; i < this->device_count; i++) {
    
    //strncmp
    // if (!this->device_list[i]->topic_listen.equals(topic)) continue;
    // switch (this->device_list[i]->type){

    //   case Device::DevType_e::Light:{
    //     if (payload.equals(String("ON"))){ this->device_list[i]->turnOn(); };
    //     if (payload.equals(String("OFF"))){ this->device_list[i]->turnOff(); };
    //     this->mqtt_publish(this->device_list[i]);
    //     break;
    //   }

    // };
  };

  // trigger callback
  if(this->mqttTopicReceivedCb != nullptr) this->mqttTopicReceivedCb(topic, payload);

};

void WebApi::mqtt_publish(const char* _topic, const char* _message){
  Serial.printf("mqtt publish | %s: %s\n", _topic, _message);
  this->mqtt->publish(_topic, _message, true);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// update device

void WebApi::mqtt_publish(Device* dev){
  const char* topic = dev->topic.c_str();
  const char* msg = dev->isOn?"ON":"OFF";

  switch (dev->type){

    // case Device::DevType_e::Light:

    //   break;

    default:
      const char* topic = dev->topic.c_str();
      const char* msg = dev->isOn?"ON":"OFF";

      this->mqtt_publish(dev->topic.c_str(), dev->isOn?"ON":"OFF");
      break;

  };

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void WebApi::mqtt_publish(Fan* fan){


  this->mqtt_publish(fan->topic_status.c_str(), fan->isOn?"ON":"OFF");
  this->mqtt_publish(fan->topic_mode.c_str(), fan->mode.c_str());
  this->mqtt_publish(fan->topic_speed.c_str(), String(fan->speed).c_str());
};


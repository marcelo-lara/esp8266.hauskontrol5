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
        
        // set topics
        ((Fan*)this->device_list[i])->topic = String(ctrl_root + "fan");
        ((Fan*)this->device_list[i])->topic_status = String(ctrl_root + "fan/status");
        ((Fan*)this->device_list[i])->topic_speed  = String(ctrl_root + "fan/speed");
        ((Fan*)this->device_list[i])->topic_mode   = String(ctrl_root + "fan/mode");

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
            this->mqtt_subscribe(String(fan->topic_status + "/set").c_str());
            this->mqtt_subscribe(String(fan->topic_speed + "/set").c_str());
            this->mqtt_subscribe(String(fan->topic_mode + "/set").c_str());
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
std::string tail(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
} // tail

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
    const char* dev_topic = this->device_list[i]->topic.c_str();
    if(strncmp(dev_topic, topic, strlen(dev_topic)) != 0 ) continue;

    String cmd = tail(topic, strlen(topic) - strlen(dev_topic)).c_str();
    Serial.print("  -> ");
    Serial.println( cmd );
    switch (this->device_list[i]->type){

      case Device::DevType_e::Light:{
        if (payload.equals(String("ON"))){ this->device_list[i]->turnOn(); };
        if (payload.equals(String("OFF"))){ this->device_list[i]->turnOff(); };
        this->mqtt_publish(this->device_list[i]);
        break;
      }
      case Device::DevType_e::Fan:{
        this->mqtt_handle_callback((Fan*)this->device_list[i], cmd, payload);
        break;
      }

    };
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

    default:
      const char* topic = dev->topic.c_str();
      const char* msg = dev->isOn?"ON":"OFF";

      this->mqtt_publish(dev->topic.c_str(), dev->isOn?"ON":"OFF");
      break;

  };

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// fan
void WebApi::mqtt_publish(Fan* fan){
  this->mqtt_publish(fan->topic_status.c_str(), fan->isOn?"ON":"OFF");
  this->mqtt_publish(fan->topic_mode.c_str(), fan->mode.c_str());
  this->mqtt_publish(fan->topic_speed.c_str(), String(fan->speed).c_str());
};

void WebApi::mqtt_handle_callback(Fan* fan, String cmd, String payload){
  
  if(cmd == "/status/set"){
    if (payload == "ON") fan->turnOn();
    if (payload == "OFF") fan->turnOff();
    return;
  };

  if(cmd == "/speed/set"){
    if (payload == "1") fan->setSpeed(1);
    if (payload == "2") fan->setSpeed(2);
    if (payload == "3") fan->setSpeed(3);
    if (payload == "4") fan->setSpeed(4);
    if (payload == "5") fan->setSpeed(5);
    return;
  };

  if(cmd == "/mode/set"){
    fan->setMode(payload);
    return;
  };

  Serial.print("  unhandled: ");
  Serial.print(cmd);
  Serial.print(" > ");
  Serial.println(payload);
}
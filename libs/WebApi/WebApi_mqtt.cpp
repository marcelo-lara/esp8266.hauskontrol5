#include "WebApi.h"
////////////////////////
// MQTT blocks

#define MQTT_CLIENT_ID "office_light1"
#define MQTT_SERVER_IP "192.168.1.201"
#define MQTT_SERVER_PORT 1883
#define MQTT_USER "darkblue"
#define MQTT_PASSWORD "mju76yhn"
WiFiClient wifiClient;

// Topic definitions


void WebApi::setup_mqtt(){
  this->mqtt = new PubSubClient(wifiClient);
  this->mqtt->setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  this->mqtt->setCallback([this] (char* topic, byte* payload, unsigned int length) { this->mqtt_callback(topic, payload, length); });

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
  this->mqtt_publish();
};

void WebApi::mqtt_publish() {
  this->mqtt->publish("hauskontrol/office/light/main", "ON", true);
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

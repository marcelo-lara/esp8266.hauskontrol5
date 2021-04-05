#include <Arduino.h>
#include "src/wemos.setup/wemos.setup.h"
#include "src/Core/Web/WebUi.h"

/////////////////////////////////////////
// Hardware Setup
#define wallSwitchPin  14 // D5 Wall Switch
#define statusLedPin   12 // D6 Wall StatusLed
#define relayOutPin    13 // D7 relay out
#define irSendPin       0 // D3 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led

#define NODE_NAME "dummyoffice"
WebUi ui(Controller::Office, NODE_NAME);

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
WiFiClient wifiClient;
PubSubClient client(wifiClient);

//// devices ////
#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(relayOutPin, true);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "office_light1";
const PROGMEM char* MQTT_SERVER_IP = "192.168.1.201";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "darkblue";
const PROGMEM char* MQTT_PASSWORD = "mju76yhn";

// MQTT: topics
const char* MQTT_LIGHT_STATE_TOPIC = "office/light1/status";
const char* MQTT_LIGHT_COMMAND_TOPIC = "office/light1/switch";
// payloads by default (on/off)
const char* LIGHT_ON = "ON";
const char* LIGHT_OFF = "OFF";

void mqtt_callback(char* p_topic, byte* p_payload, unsigned int p_length) {
  // concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }
  
  Serial.print("mqtt> ");
  Serial.print(p_topic);
  Serial.print(" | ");
  Serial.println(payload);

  if (String("homeassistant/light/office_light/state").equals(p_topic)) {
      client.publish("homeassistant/light/office_light/state", "ON");
  }

  // handle message topic
  if (String(MQTT_LIGHT_COMMAND_TOPIC).equals(p_topic)) {
    // test if the payload is equal to "ON" or "OFF"
    if (payload.equals(String(LIGHT_ON))) {
        Serial.println("lightOn packet");
    } else if (payload.equals(String(LIGHT_OFF))) {
        Serial.println("lightOn packet");
    }
  }

}

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=[](int cc) {light.toggle();};
  light.turnOn();

  //connect
  wemosWiFi.connect(NODE_NAME);

  //mqtt connect
  client.setServer("192.168.1.201", 1883);
  client.setCallback(mqtt_callback);

  // Start the server
  ui.add_device(&light);
  ui.add_device(&environment);
  ui.init();
 
  //non-critical hardware
  light.turnOff();
  environment.setup();
  digitalWrite(statusLedPin, HIGH);

}
 
void loop() {
  wallSwitch.update();
  environment.update();

  wemosWiFi.update();
  ui.update();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("dummyoffice", "darkblue", "mju76yhn")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("homeassistant/dummynode/light/state","ON");
      
      char* hass_config = "{\"~\": \"homeassistant/light/office\",  \"name\": \"Office Light\",  \"unique_id\": \"ctrl_office_light\",  \"cmd_t\": \"~/set\",  \"stat_t\": \"~/state\",  \"schema\": \"json\",  \"brightness\": true}";
      client.publish("homeassistant/light/office/config", hass_config);
      // ... and resubscribe
      client.subscribe("homeassistant/light/office/state");
      client.subscribe("homeassistant/light/office/set");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
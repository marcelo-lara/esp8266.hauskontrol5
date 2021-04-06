#pragma once
#include "Arduino.h"
typedef void MqttCallback(char* p_topic, byte* p_payload, unsigned int p_length);

class MqttClient{
public:
    void setup();
    void update();

    //MqttCallback *mqttCallback;
    void reconnect();
};

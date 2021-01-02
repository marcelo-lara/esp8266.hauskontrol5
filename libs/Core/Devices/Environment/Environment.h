#pragma once
#include "Arduino.h"

class Environment {
public:
    
    // values
    float temperature;
    float humidity;
    float pressure;
    float light;

    // device
    void setup();
    void update();

    bool isOn;

    // json
    String to_json(){
        return \
        "{\
            \"temp\":\"" + String(this->temperature) + "\", \
            \"hum\":\"" + String(this->humidity) + "\", \
            \"pres\":\"" + String(this->pressure) + "\", \
            \"light\":\"" + String(this->light) + "\" \
        }";
    }

private:
    long lastUpdated;

};

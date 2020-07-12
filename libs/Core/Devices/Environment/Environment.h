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

private:
    long lastUpdated;

};

#pragma once
#include "Arduino.h"
typedef void LightStatusChanged(int newStatus);

class Light {
public:
    Light(int _pin);
    Light(int _pin, bool invertOnStatus);
    void turnOn();
    void turnOff();
    void toggle();

    bool isOn;

    // json
    String to_json(){
        return "\"light\":\"" + String(this->isOn?"1":"0") + "\"";
    }

private:
    int pin;
    bool OnIsLow;
    void setOutput(bool newStatus);

};

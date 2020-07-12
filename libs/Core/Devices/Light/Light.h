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
private:
    int pin;
    bool outOn;
    void setOutput(bool newStatus);

};

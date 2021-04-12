#pragma once
#include "Arduino.h"

class Device {
public:
    enum DevType_e {AC, Environment, Fan, Light};
    DevType_e type;
    Device():Device(Light){};
    Device(DevType_e _dev_type){this->type=_dev_type;};


    String name;

    virtual void turnOn(){};
    virtual void turnOff(){};
    virtual void toggle(){};
    bool isOn=false;

};
#pragma once
#include "Arduino.h"

class Device {
public:
    enum DevType_e {NotSet, AC, Environment, Fan, Light, Button};
    DevType_e type;
    Device():Device(NotSet){};
    Device(DevType_e _dev_type){this->type=_dev_type;};

    String name;

    virtual void turnOn(){};
    virtual void turnOff(){};
    virtual void toggle(){};
    bool isOn=false;

    String topic;
    String topic_listen;

};
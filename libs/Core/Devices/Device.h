#pragma once
#include "Arduino.h"

class Device {
public:
    enum DevType_e {NotSet, AC, Environment, Fan, Light, Button};
    DevType_e type;
    Device():Device(NotSet){};
    Device(DevType_e _dev_type):Device(_dev_type, ""){};
    Device(DevType_e _dev_type, String _name){this->type=_dev_type;this->name=_name;};

    String name;

    virtual void turnOn(){};
    virtual void turnOff(){};
    virtual void toggle(){};
    bool isOn=false;

    String topic;
    String topic_listen;
    
    // virtual device (ie mqtt)
    bool isVirtual;
};
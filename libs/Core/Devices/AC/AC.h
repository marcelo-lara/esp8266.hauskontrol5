#pragma once
#include "Arduino.h"
typedef void ACStatusChanged(bool isOn);

class AC {
public:
    AC(int _irOut, int defaultTemp);
    
    void init();

    //base
    void turnOn();
    void turnOff();
    void toggle();
    bool isOn;

    //temp
    void setTemp(uint _temp);
    uint  temp;
   
    //fan/flow
    uint  flow;
    void setFlow(uint _flow_level);

    //swing
    bool swing;
    void swingOn();
    void swingOff();

    // json
    String to_json(){
        return \
        "{\
            \"power\":\"" + String(this->isOn)  + "\", \
            \"temp\":\""  + String(this->temp)  + "\", \
            \"flow\":\""  + String(this->flow)  + "\", \
            \"swing\":\"" + String(this->swing) + "\" \
        }";
    }

private:
    int irOut;
};

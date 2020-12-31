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

private:
    int irOut;
};

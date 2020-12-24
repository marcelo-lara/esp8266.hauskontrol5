#pragma once
#include "Arduino.h"
typedef void ACStatusChanged(bool isOn);

class AC {
public:
    AC(int _irOut, int defaultTemp);
    
    //base
    void turnOn();
    void turnOff();
    void toggle();
    bool isOn;

    //details
    void setTemp(int _temp);
    int  temp;

private:
    int irOut;
};

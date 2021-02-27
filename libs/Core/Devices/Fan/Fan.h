#pragma once
#include "Arduino.h"
#include "../../Io/ShiftedIo/ShiftedIo.h"
typedef void FanStatusChanged(int outSpeed);

class Fan {
public:
    Fan(int _pin1,int _pin2,int _pin3,int _pin4, int defaultSpeed);
    Fan(ShiftedIo _shiftedOut, int defaultSpeed);
    void turnOn();
    void turnOff();
    void toggle();
    void setSpeed(int _speed);

    bool isOn;
    int  speed;
    
private:
    int pin1;
    int pin2;
    int pin3;
    int pin4;
    int onSpeed;
    void setOutput(int fanSpeed);

};

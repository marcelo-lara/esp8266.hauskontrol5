#pragma once
#include "Arduino.h"
#include "../Device.h"
#include "../../Io/ShiftedIo/ShiftedIo.h"

typedef void FanStatusChanged(int outSpeed);

class Fan : public Device {
public:
    Fan(int _pin1,int _pin2,int _pin3,int _pin4, int _defaultSpeed);
    Fan(ShiftedIo *_shiftedOut, int defaultSpeed);
    void turnOn();
    void turnOff();
    void toggle();
    void setSpeed(int _speed);
    int  speed;
    int  max_speed;
    bool isOn;

    String mode = "auto";
    void setMode(String _mode){this->mode=_mode; if(this->statusChanged != nullptr) this->statusChanged(speed);};

    FanStatusChanged* statusChanged;

    //mqtt
    String topic_status;
    String topic_speed;
    String topic_mode;

private:
    int  onSpeed;
    void render();

    //direct out
    int pin1;
    int pin2;
    int pin3;
    int pin4;
    void render_direct();

    //shifted out
    bool isShiftedOut;
    ShiftedIo *shiftedOut;
    void render_shifted();

};

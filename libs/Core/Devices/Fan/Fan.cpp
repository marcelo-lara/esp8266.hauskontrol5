#include "Fan.h"

Fan::Fan(int _pin1,int _pin2,int _pin3,int _pin4, int _defaultSpeed) : Device(Device::DevType_e::Fan, "fan") {
    out[0] = _pin1;
    out[1] = _pin2;
    out[2] = _pin3;
    out[3] = _pin4;
    onSpeed=_defaultSpeed;
    max_speed=4;
};
Fan::Fan(bool _inv_out, int _pin1,int _pin2,int _pin3,int _pin4, int _defaultSpeed) : Device(Device::DevType_e::Fan, "fan") {
    inv_out = _inv_out;
    out[0] = _pin1;
    out[1] = _pin2;
    out[2] = _pin3;
    out[3] = _pin4;
    onSpeed=_defaultSpeed;
    max_speed=4;
};
Fan::Fan(ShiftedIo *_shiftedOut, int _defaultSpeed) : Device(Device::DevType_e::Fan, "fan"){
    this->shiftedOut = _shiftedOut;
    this->isShiftedOut = true;
    this->onSpeed=_defaultSpeed;
    max_speed=5;
};

void Fan::turnOn(){
    speed=onSpeed;
    this->render();
};
void Fan::turnOff(){
    speed=0;
    this->render();
};
void Fan::toggle(){
    if(isOn) turnOff();
    else     turnOn();
};
void Fan::setSpeed(int _speed){
    speed = _speed;
    onSpeed = _speed;
    this->render();
};


///////////////////////////////////////////////////////////////////
// Hardware IO
void Fan::render(){
    isOn=speed>0;

    if(isShiftedOut)
        render_shifted();
    else
        render_direct();

    Serial.printf("fan| speed %i\n", speed);
    if(this->statusChanged != nullptr) 
        this->statusChanged(speed);

};

void Fan::render_direct(){
    for (int i = 0; i < this->max_speed; i++) {
        pinMode(this->out[i], OUTPUT);
        digitalWrite(this->out[i], ((this->speed > i) ? (this->inv_out?0:1) : (this->inv_out?1:0)) );
    }
};
void Fan::render_shifted(){
    for (int i = 0; i < this->max_speed; i++)
        this->shiftedOut->setIo(i, speed>i, true);
    this->shiftedOut->render();
};

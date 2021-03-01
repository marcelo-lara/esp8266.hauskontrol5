#include "Fan.h"

Fan::Fan(int _pin1,int _pin2,int _pin3,int _pin4, int _defaultSpeed){
    pin1=_pin1; pinMode(pin1, OUTPUT); digitalWrite(pin1, 0);
    pin2=_pin2; pinMode(pin2, OUTPUT); digitalWrite(pin2, 0);
    pin3=_pin3; pinMode(pin3, OUTPUT); digitalWrite(pin3, 0);
    pin4=_pin4; pinMode(pin4, OUTPUT); digitalWrite(pin4, 0);
    onSpeed=_defaultSpeed;
};
Fan::Fan(ShiftedIo *_shiftedOut, int _defaultSpeed){
    this->shiftedOut = _shiftedOut;
    this->isShiftedOut = true;
    this->onSpeed=_defaultSpeed;
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
int Fan::getSpeed(){
    return this->onSpeed;
};


///////////////////////////////////////////////////////////////////
// Hardware IO
void Fan::render(){

    isOn=speed>0;
    Serial.printf("set_fan: %i\n", speed);

    if(isShiftedOut)
        render_shifted();
    else
        render_direct();
};

void Fan::render_direct(){
    switch (speed){
        case 0: digitalWrite(pin1, 0); digitalWrite(pin2, 0); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
        case 1: digitalWrite(pin1, 1); digitalWrite(pin2, 0); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
        case 2: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
        case 3: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 1); digitalWrite(pin4, 0); break;
        case 4: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 1); digitalWrite(pin4, 1); break;
    };
};
void Fan::render_shifted(){
    for (int i = 0; i < 5; i++)
        this->shiftedOut->setIo(i, speed>i, true);

    this->shiftedOut->render();


};
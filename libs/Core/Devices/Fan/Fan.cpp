#include "Fan.h"

Fan::Fan(int _pin1,int _pin2,int _pin3,int _pin4, int defaultSpeed){
    pin1=_pin1; pinMode(pin1, OUTPUT); digitalWrite(pin1, 0);
    pin2=_pin2; pinMode(pin2, OUTPUT); digitalWrite(pin2, 0);
    pin3=_pin3; pinMode(pin3, OUTPUT); digitalWrite(pin3, 0);
    pin4=_pin4; pinMode(pin4, OUTPUT); digitalWrite(pin4, 0);
    onSpeed=defaultSpeed;
};

void Fan::turnOn(){
    setOutput(onSpeed);
};
void Fan::turnOff(){
    setOutput(0);
};
void Fan::toggle(){
    if(isOn) turnOff();
    else     turnOn();
};
void Fan::setSpeed(int _speed){
    setOutput(_speed);
};

void Fan::setOutput(int fanSpeed){
    speed=fanSpeed;
    isOn=fanSpeed>0;
    if(isOn) onSpeed=fanSpeed;
    Serial.printf("fanOut: %i\n", fanSpeed);
    switch (fanSpeed)
    {
    case 0: digitalWrite(pin1, 0); digitalWrite(pin2, 0); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
    case 1: digitalWrite(pin1, 1); digitalWrite(pin2, 0); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
    case 2: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
    case 3: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 1); digitalWrite(pin4, 0); break;
    case 4: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 1); digitalWrite(pin4, 1); break;
    default:
        break;
    }
};
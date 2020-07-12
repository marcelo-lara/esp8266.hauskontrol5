#include "Light.h"

Light::Light(int _pin){
    pin=_pin;
    pinMode(pin, OUTPUT);
};
Light::Light(int _pin, bool invertOnStatus){
    pin=_pin;
    pinMode(pin, OUTPUT);
    outOn=!invertOnStatus;
};


void Light::turnOn(){
    setOutput(true);
};
void Light::turnOff(){
    setOutput(false);
};
void Light::toggle(){
    setOutput(!isOn);
};
void Light::setOutput(bool newStatus){
    isOn=newStatus;
    digitalWrite(pin, isOn? HIGH : LOW);
};
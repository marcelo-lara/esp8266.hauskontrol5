#include "Light.h"

Light::Light(int _pin){
    pin=_pin;
};
Light::Light(int _pin, bool invertOnStatus){
    Light("light", _pin, invertOnStatus);
};
Light::Light(String _name, int _pin, bool invertOnStatus){
    pinMode(pin, OUTPUT);
    name=_name;
    pin=_pin;
    OnIsLow=invertOnStatus;
}

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
    pinMode(pin, OUTPUT);
    digitalWrite(pin, OnIsLow ? (isOn? LOW: HIGH) : (isOn? HIGH: LOW ));
};
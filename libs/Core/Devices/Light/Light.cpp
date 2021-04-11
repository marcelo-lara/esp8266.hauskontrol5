#include "Light.h"

Light::Light(int _pin) : Light("main", _pin, false){};
Light::Light(int _pin, bool invertOnStatus) : Light("main", _pin, invertOnStatus){};
Light::Light(String _name, int _pin, bool invertOnStatus){
    pinMode(pin, OUTPUT);
    name=_name;
    pin=_pin;
    OnIsLow=invertOnStatus;
}
Light::Light(ShiftedIo *_shiftedOut, int _bus_position):Light("main", _shiftedOut, _bus_position, false){};
Light::Light(String _name, ShiftedIo *_shiftedOut, int _bus_position, bool invertOnStatus){
    this->name=_name;
    this->isShiftedOut=true;
    this->shiftedOut=_shiftedOut;
    this->bus_position=_bus_position;
    this->OnIsLow=invertOnStatus;
};

void Light::turnOn(){
    Serial.println("Light::turnOn()");
    setOutput(true);
};
void Light::turnOff(){
    Serial.println("Light::turnOn()");
    setOutput(false);
};
void Light::toggle(){
    setOutput(!isOn);
};
void Light::setOutput(bool newStatus){
    isOn=newStatus;
    if(this->isShiftedOut){
        //shifted render 
        this->shiftedOut->setIo(this->bus_position, this->isOn);
    }else{
        //direct render
        pinMode(pin, OUTPUT);
        digitalWrite(pin, OnIsLow ? (isOn? LOW: HIGH) : (isOn? HIGH: LOW ));
    }

};
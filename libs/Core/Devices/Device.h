#pragma once

class Device {
public:
    String name;

    virtual void turnOn(){
        Serial.println("Device::turnOn()");
    };
    virtual void turnOff(){
        Serial.println("Device::turnOff()");
    };
    virtual void toggle(){
        Serial.println("Device::toggle()");
    };
    bool isOn=false;

};
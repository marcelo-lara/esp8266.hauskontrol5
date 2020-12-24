#include "../Core/Devices/AC/AC.h"

// git@github.com:crankyoldgit/IRremoteESP8266.git
#include <IRremoteESP8266.h>

AC::AC(int _irOut, int defaultTemp) 
{
    
}

void AC::turnOn() 
{
    this->isOn=true;
}

void AC::turnOff() 
{
    this->isOn=false;
}

void AC::toggle() 
{
    this->isOn=!(this->isOn);
}

void AC::setTemp(int _temp) 
{
    
}

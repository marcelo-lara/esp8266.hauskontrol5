#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE    DHT11
#define DHTPIN 2

// #define I2C_ADDRESS 0x76
// BMx280I2C bmx280(I2C_ADDRESS);  // requires git@github.com:marcelo-lara/BMx280MI-1.git

#include "../Core/Devices/Environment/Environment.h"

bool _measuring = false;

void Environment::setup(){
    Serial.print("DHT11 Environment| ");
    this->isOn=false;

    lastUpdated=0;
    this->isOn=true;

    Serial.println("faked!");
};

void Environment::update(){
    //sensor not present: ignore measurements
    if(lastUpdated==-1) return;

    //already measured, skip
    if(millis()<lastUpdated) return;

	//measure

    //update values
    temperature = 20;
    pressure    = 1000;
    humidity    = 50;

    //wait until next read
    _measuring=false;
    if(this->statusUpdated != nullptr) this->statusUpdated(this->isOn);

    lastUpdated = millis()+5000;
};


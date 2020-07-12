#include <Arduino.h>
#include <Wire.h>
#include <BMx280I2C.h>
#define I2C_ADDRESS 0x76
BMx280I2C bmx280(I2C_ADDRESS);

#include "../Core/Devices/Environment/Environment.h"

bool _measuring = false;

void Environment::setup(){
    Serial.print("Environment| ");
	Wire.begin();
	if (!bmx280.begin()){
        lastUpdated=-1;
        Serial.println("sensor failed");
		return;
	}
	//reset sensor to default parameters.
	bmx280.resetToDefaults();

	//by default sensing is disabled and must be enabled by setting a non-zero
	//oversampling setting.
	//set an oversampling setting for pressure and temperature measurements. 
	bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
	bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

	//if sensor is a BME280, set an oversampling setting for humidity measurements.
	if (bmx280.isBME280())
		bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

    lastUpdated=0;

    Serial.println("online");
};
void Environment::update(){
    //sensor not present: ignore measurements
    if(lastUpdated==-1) return;

    //already measured, skip
    if(millis()<lastUpdated) return;

	//measure
    if (!_measuring){
        bmx280.measure();
        _measuring=true;
        return;
    } 

    //update values
    if (!bmx280.hasValue()) return;
    temperature = bmx280.getTemperature();
    pressure    = bmx280.getPressure()/100;
    humidity    = bmx280.getHumidity();

    //wait until next read
    _measuring=false;
    lastUpdated = millis()+1000;
};


#pragma once
#include "Arduino.h"
#include "../Device.h"
typedef void ACStatusChanged(bool isOn);

class AC : public Device {
public:
    AC(int _irOut, int defaultTemp);
    
    void init();

    //home-assistant values https://www.home-assistant.io/integrations/climate.mqtt/
    // String configuration_url;
    // String identifiers;
    // String manufacturer;
    // String model;
    // String name;
    // String suggested_area;
    // String sw_version;
    int	   max_temp;
    int	   min_temp;
    // float  current_temperature;
    // float  current_humidity;
    // float  target_temperature;
    // float  target_temperature_high;
    // float  target_temperature_low;
    // float  target_temperature_step;
    // float  target_humidity;
    // String	hvac_mode;
    // String	hvac_action;
    // //list	hvac_modes;
    // String	preset_mode;
    // //list	preset_modes;
    // String	fan_mode;
    // //list	fan_modes;
    // String	swing_mode;
    // //list	swing_modes;


    String temperature_unit;
    float  precision;

    String fan_mode_command_topic;
    


    //base
    void turnOn();
    void turnOff();
    void toggle();
    bool isOn;

    //temp
    void setTemp(uint _temp);
    uint  temp;
   
    //fan/flow
    uint  flow;
    void setFlow(uint _flow_level);

    //swing
    bool swing;
    void swingOn();
    void swingOff();

    // json
    String to_json(){
        return \
        "{\
            \"power\":\"" + String(this->isOn)  + "\", \
            \"temp\":\""  + String(this->temp)  + "\", \
            \"flow\":\""  + String(this->flow)  + "\", \
            \"swing\":\"" + String(this->swing) + "\" \
        }";
    };

private:
    int irOut;
};

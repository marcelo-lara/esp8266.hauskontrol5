#pragma once
#include "Arduino.h"
#include "../Device.h"

class Environment : public Device {
public:
    
    Environment() : Device(Device::DevType_e::Environment, "environment"){};

    // values
    float temperature;
    float humidity;
    float pressure;
    float light;

    // device
    void setup();
    void update();

    bool isOn;

    // json
    String to_json(){
        return "\
            \"temp\":\"" + String(this->temperature) + "\", \
            \"hum\":\"" + String(this->humidity) + "\", \
            \"pres\":\"" + String(this->pressure) + "\", \
            \"light\":\"" + String(this->light) + "\"";
    };

    // html
    String to_html(){
      String dev_html;
      dev_html += "<div class=\"block env temp\">" + String(this->temperature) + "</div>";
      dev_html += "<div class=\"block env humidity\">" + String(this->humidity) + "</div>";
      dev_html += "<div class=\"block env pressure\">" + String(this->pressure) + "</div>";
      dev_html += "<div class=\"block env light\">" + String(this->light) + "</div>";
      return dev_html;
    };

private:
    long lastUpdated;

};

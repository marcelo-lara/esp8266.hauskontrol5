#pragma once
#include "Arduino.h"
#include "../Device.h"
typedef void EnvStatusUpdated(bool state);

class Environment : public Device {
public:
    
    Environment() : Device(Device::DevType_e::Environment, "environment"){};

    // values
    float temperature;
    float humidity;
    float pressure;
    float illuminance;

    // device
    void setup();
    void update();

    EnvStatusUpdated *statusUpdated;

    bool isOn;

    // json
    String to_json(){
        return "\
            \"temp\":\"" + String(this->temperature) + "\", \
            \"hum\":\"" + String(this->humidity) + "\", \
            \"pres\":\"" + String(this->pressure) + "\", \
            \"light\":\"" + String(this->illuminance) + "\"";
    };

    // html
    String to_html(){
      String dev_html;
      dev_html += "<div class=\"block env temp\">" + String(this->temperature) + "</div>";
      dev_html += "<div class=\"block env humidity\">" + String(this->humidity) + "</div>";
      dev_html += "<div class=\"block env pressure\">" + String(this->pressure) + "</div>";
      dev_html += "<div class=\"block env light\">" + String(this->illuminance) + "</div>";
      return dev_html;
    };

    // mqtt
    String topic_temperature;
    String topic_humidity;
    String topic_pressure;
    String topic_illuminance;

private:
    long lastUpdated;

};

#pragma once
#include "Arduino.h"
typedef void LightStatusChanged(int newStatus);

class Light {
public:
    Light(int _pin);
    Light(int _pin, bool invertOnStatus);
    Light(String _name, int _pin, bool invertOnStatus);
    void turnOn();
    void turnOff();
    void toggle();

    bool isOn;
    String name;

    // json
    String to_json(){
        return "\"" + String(this->name) + "\":\"" + String(this->isOn?"1":"0") + "\"";
    }

    // html
    String to_html_div(){
      String dev_html;
      dev_html += "<div";
      dev_html += " class=\"block button light " + String(this->isOn?"on":"off") + "\"";
      dev_html += " target=\"set/light/" + this->name + "/toggle\"";
      dev_html += ">";
      dev_html += String(this->name);
      dev_html += "</div>";
      return dev_html;
    }

private:
    int pin;
    bool OnIsLow;
    void setOutput(bool newStatus);

};

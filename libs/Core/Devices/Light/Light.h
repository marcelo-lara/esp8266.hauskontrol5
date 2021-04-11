#pragma once
#include "Arduino.h"
#include "../Device.h"
#include "../../Io/ShiftedIo/ShiftedIo.h"
typedef void LightStatusChanged(int newStatus);

class Light : public Device {
public:
    Light(int _pin);
    Light(int _pin, bool invertOnStatus);
    Light(String _name, int _pin, bool invertOnStatus);
    Light(ShiftedIo *_shiftedOut, int _bus_position);
    Light(String _name, ShiftedIo *_shiftedOut, int _bus_position, bool invertOnStatus);


    void turnOn();
    void turnOff();
    void toggle();

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

    //shifted out
    bool isShiftedOut;
    int bus_position;
    ShiftedIo *shiftedOut;

};

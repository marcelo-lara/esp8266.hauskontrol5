#pragma once
#include "Arduino.h"
#include "../Device.h"
#include "../../Io/ShiftedIo/ShiftedIo.h"
typedef void LightStatusChanged(String topic, bool state);

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

    LightStatusChanged* statusChanged;

private:

    int pin;
    bool OnIsLow;
    void setOutput(bool newStatus);

    //shifted out
    bool isShiftedOut;
    int bus_position;
    ShiftedIo *shiftedOut;

};

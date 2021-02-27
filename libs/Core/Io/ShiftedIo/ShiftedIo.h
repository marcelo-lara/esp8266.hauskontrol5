#pragma once
#include "Arduino.h"
typedef void ShiftedIoChanged(int _out, int _state);

class ShiftedIo {
public:
    ShiftedIo(int _latchPin, int _dataPin, int _clockPin, bool _invertedOut);
    
    void setIo(int _out, bool _state);
    bool getIo(int _out);
    void toggleIo(int _out);


private:
    int __latchPin;
    int __dataPin;
    int __clockPin;
    bool __invertedOut;
    void render();
    int __registerOutput;
};

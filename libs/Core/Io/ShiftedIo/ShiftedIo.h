#pragma once
#include "Arduino.h"
typedef void ShiftedIoChanged(int _out, int _state);

class ShiftedIo {
public:
    ShiftedIo(int _latchPin, int _dataPin, int _clockPin, bool _invertedOut);
    
    void setIo(int _out, bool _state);
    void setIo(int _out, bool _state, bool _hold_render);
    bool getIo(int _out);
    void toggleIo(int _out);
    void render();


private:
    int __latchPin;
    int __dataPin;
    int __clockPin;
    bool __invertedOut;
    int __registerOutput;
};

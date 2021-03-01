#include "ShiftedIo.h"

ShiftedIo::ShiftedIo(int _latchPin, int _dataPin, int _clockPin, bool _invertedOut){

  //store local connection
  __latchPin  = _latchPin;
  __dataPin   = _dataPin;
  __clockPin  = _clockPin;
  __invertedOut = _invertedOut;

  //pin setup / reset
  pinMode(__latchPin, OUTPUT);
  pinMode(__dataPin, OUTPUT);
  pinMode(__clockPin,  OUTPUT);
  digitalWrite(__dataPin,  LOW);
  digitalWrite(__clockPin, LOW);

  //set register status
  this->__registerOutput = __invertedOut?255:0;
  this->render();

};

void ShiftedIo::setIo(int _out, bool _state){
  this->setIo(_out, _state, false);
};

void ShiftedIo::setIo(int _out, bool _state, bool _hold_render){
  if(__invertedOut)
    _state = !_state;

  if(_state){
    bitSet(__registerOutput, _out);
  }else{
    bitClear(__registerOutput, _out);
  }
  
  if(!_hold_render)
    this->render();
};
bool ShiftedIo::getIo(int _out){

  bool val = bitRead(__registerOutput, _out) == 1;

  if(__invertedOut)
    val = !val;

  return val;

};
void ShiftedIo::toggleIo(int _out){
  bool val = bitRead(__registerOutput, _out) == 1;
  this->setIo(_out, !val);
};

void ShiftedIo::render(){
    digitalWrite(__latchPin, LOW);
    shiftOut(__dataPin, __clockPin, MSBFIRST, this->__registerOutput);
    digitalWrite(__latchPin, HIGH);

    Serial.print("\tshift_render: 0x");
    Serial.println(__registerOutput, BIN);

};
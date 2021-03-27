#include "Fan.h"

Fan::Fan(int _pin1,int _pin2,int _pin3,int _pin4, int _defaultSpeed){
    pin1=_pin1; pinMode(pin1, OUTPUT); digitalWrite(pin1, 0);
    pin2=_pin2; pinMode(pin2, OUTPUT); digitalWrite(pin2, 0);
    pin3=_pin3; pinMode(pin3, OUTPUT); digitalWrite(pin3, 0);
    pin4=_pin4; pinMode(pin4, OUTPUT); digitalWrite(pin4, 0);
    onSpeed=_defaultSpeed;
    max_speed=4;
};
Fan::Fan(ShiftedIo *_shiftedOut, int _defaultSpeed){
    this->shiftedOut = _shiftedOut;
    this->isShiftedOut = true;
    this->onSpeed=_defaultSpeed;
    max_speed=5;
};

void Fan::turnOn(){
    speed=onSpeed;
    this->render();
};
void Fan::turnOff(){
    speed=0;
    this->render();
};
void Fan::toggle(){
    if(isOn) turnOff();
    else     turnOn();
};
void Fan::setSpeed(int _speed){
    speed = _speed;
    onSpeed = _speed;
    this->render();
};


///////////////////////////////////////////////////////////////////
// Hardware IO
void Fan::render(){

    isOn=speed>0;
    Serial.printf("set_fan: %i\n", speed);

    if(isShiftedOut)
        render_shifted();
    else
        render_direct();
};

void Fan::render_direct(){
    pinMode(pin1, OUTPUT);pinMode(pin2, OUTPUT);pinMode(pin3, OUTPUT);pinMode(pin4, OUTPUT);
    switch (speed){
        case 0: digitalWrite(pin1, 0); digitalWrite(pin2, 0); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
        case 1: digitalWrite(pin1, 1); digitalWrite(pin2, 0); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
        case 2: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 0); digitalWrite(pin4, 0); break;
        case 3: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 1); digitalWrite(pin4, 0); break;
        case 4: digitalWrite(pin1, 1); digitalWrite(pin2, 1); digitalWrite(pin3, 1); digitalWrite(pin4, 1); break;
    };
};
void Fan::render_shifted(){
    for (int i = 0; i < this->max_speed; i++)
        this->shiftedOut->setIo(i, speed>i, true);
    this->shiftedOut->render();
};


////////////////////////////////////////////////////////////
// api

String Fan::to_json(){
    return "\"speed\":\"" + String(this->speed) + "\"";
};
String Fan::to_html(){
    String dev_html;

    for (int i = 0; i < this->max_speed+1; i++){
        dev_html += "<div";
        if(i==0){
            dev_html += " class=\"block button fan " + String(this->isOn?"on":"off") + "\"";
            dev_html += " target=\"set/fan/" + String(this->isOn?"off":"on")  + "\"";
            dev_html += ">";
            dev_html += "fan";
        }else{
            dev_html += " class=\"block button fan " + String(i==this->speed?"on":"") + "\"";
            dev_html += " target=\"set/fan/speed/" + String(i) + "\"";
            dev_html += ">";
            dev_html += String(i);
        };
        dev_html += "</div>";
    }
    
    return dev_html;
};
#define inSwitch_debounce 20
#define inSwitch_click 300
#define inSwitch_longPress 1000
#define inSwitch_timeout 400

#define btn_down 0
#define btn_up 1

#include "Button.h"
int _lastStatus;
int _currStatus;

Button::Button(uint8_t _switchButton){
    inSwitch=_switchButton;
    pinMode(inSwitch, INPUT_PULLUP);
	_lastStatus=digitalRead(inSwitch);
};


void Button::setup(){}

unsigned long _sw_timerStart;
unsigned long _sw_nextClickTimeout;
unsigned long _sw_longPressTimeout;
unsigned long _sw_minNextClick;
int _sw_clickCount = 0;

void Button::update(){
	_currStatus=digitalRead(inSwitch);
	
	//are we counting clicks?
	if(_sw_clickCount>0){

		//debounce ignore
		if(_sw_minNextClick<inSwitch_debounce){
			Serial.println("debounce> ignoring");
            return;
		}

    //button released.. click timeout?
    if(_currStatus==btn_up && _sw_nextClickTimeout<millis()){
      //no more wait, send click event
      swCallback(_sw_clickCount);
      //reset counters
      _sw_clickCount=0;
      _lastStatus=_currStatus;
    }

    //it's longpress timeout?
    if(_sw_longPressTimeout<millis()){
      //send longpress event
      swCallback(-1);

      //reset counters
      _sw_clickCount=0;
      _lastStatus=_currStatus;
    }
    
	}

	//we're not counting clicks, something changed?
	if(_lastStatus==_currStatus) return;
	_lastStatus=_currStatus;

	//button is pressed
	if(_currStatus==btn_down){
		_sw_clickCount++;
		_sw_timerStart=millis();
		_sw_longPressTimeout=_sw_timerStart+inSwitch_longPress;
		_sw_nextClickTimeout=millis()+inSwitch_timeout;
		_sw_minNextClick=millis()+inSwitch_debounce;
	}

	//button is released
	else{

    //maybe released after longpress, ignoring
    if(_sw_clickCount==0) return;

    //count click
		unsigned long elapsed=millis()-_sw_timerStart;
		_sw_nextClickTimeout=millis()+inSwitch_timeout;
	}
}
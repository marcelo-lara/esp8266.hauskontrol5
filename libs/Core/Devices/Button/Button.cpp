#define inSwitch_debounce 	50		
#define inSwitch_timeout 	500
#define inSwitch_longPress 	3000	

#define btn_press 0
#define btn_release 1

#include "Button.h"
int _btn_lastStatus;
int _btn_currStatus;

Button::Button(uint8_t _switchButton){
    inSwitch=_switchButton;
    pinMode(inSwitch, INPUT_PULLUP);
	_btn_lastStatus=digitalRead(inSwitch);
};


void Button::setup(){}

unsigned long _sw_lastClick;
unsigned long _sw_pressTime;

unsigned long _sw_nextClickTimeout;
unsigned long _sw_longPressTimeout = 0;
unsigned long _sw_minNextClick = 0;
int _sw_clickCount = 0;

void Button::update(){
	_btn_currStatus=digitalRead(inSwitch);
	
	// deliver click count on release
	if(_sw_clickCount > 0 && _btn_currStatus==btn_release && millis() > _sw_nextClickTimeout) {
	    swCallback(_sw_clickCount);
		_sw_clickCount=0;
		_sw_longPressTimeout = 0;
	}

	// trap longpress
	if(_sw_longPressTimeout > 0 && _btn_currStatus==btn_press && _sw_longPressTimeout < millis()){
		_sw_clickCount=-1;
		_sw_longPressTimeout = 0;
		swCallback(_sw_clickCount);
		return;
	}

	// test current status
	if(_btn_lastStatus == _btn_currStatus) return;
	_btn_lastStatus = _btn_currStatus;
	_sw_pressTime = millis() - _sw_lastClick;


	//button position changed
	if(_btn_currStatus==btn_press){
		_sw_lastClick = millis();
		_sw_longPressTimeout = _sw_lastClick + inSwitch_longPress;
	}
	else
	{
		//ignore back from longpress
		if(_sw_clickCount==-1){
			_sw_clickCount=0;
			return;
		}
	
		if(_sw_pressTime<inSwitch_debounce) return;

		// count click
		_sw_clickCount++;
		_sw_nextClickTimeout = millis() + inSwitch_timeout;
		_sw_longPressTimeout = 0;
	}
}
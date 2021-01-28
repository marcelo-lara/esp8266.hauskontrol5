#include "../Core/Devices/AC/AC.h"

// git@github.com:crankyoldgit/IRremoteESP8266.git
#include <IRremoteESP8266.h>
#include <IRsend.h>
IRsend irsend(14);  // An IR LED is controlled by GPIO pin 14 (D5)

const unsigned int kAc_Type = 1; // 1: Wall
unsigned int        ac_heat = 0; // 0: Cool | 1: Heat
unsigned int    ac_power_on = 0; // 0: Off  | 1: On
unsigned int ac_air_clean_state = 0; // 0 : off | 1 : on --> power on
unsigned int ac_temperature = 23; // 18 ~ 30
unsigned int ac_flow = 2; // 0: low | 2: high
const uint8_t kAc_Flow_Tower[3] = {0, 4, 6};
const uint8_t kAc_Flow_Wall[4] = {0, 2, 4, 5};    

//private
void ac_update(unsigned int temperature, unsigned int air_flow);

AC::AC(int _irOut, int defaultTemp) 
{
    // defaults
    temp  = 23;
    flow  = 0;
    swing = true;
}

void AC::init(){
    Serial.println("-- AC::init() --");
    irsend.begin();
}

void AC::turnOn() 
{
    Serial.println("-- AC::turnOn() --");
    ac_update(this->temp, this->flow);
    this->isOn=true;
}

void AC::turnOff() 
{
    irsend.sendLG(0x88C0051, 28);
    this->isOn=false;
}

void AC::toggle() 
{
    if(this->isOn)
      this->turnOff();
    else
      this->turnOn();
}

////////////////////////////////////////////////////////////////////////

void AC::swingOn(){
  irsend.sendLG(0x8810001, 28);
  this->swing=true;
};
void AC::swingOff(){
  irsend.sendLG(0x8810001, 28);
  this->swing=false;
};

////////////////////////////////////////////////////////////////////////

void AC::setTemp(uint _temp) 
{
    if(_temp < 18) _temp = 18;
    if(_temp > 26) _temp = 26;
    this->temp=_temp;
    ac_update(this->temp, this->flow);
}

////////////////////////////////////////////////////////////////////////

void AC::setFlow(uint _flow){
  if(_flow>2) _flow=2;
  this->flow=_flow;
  ac_update(this->temp, this->flow);
}

uint32_t ac_code_to_sent;

void ac_update(unsigned int temperature, unsigned int air_flow) {
  unsigned int ac_msbits1 = 8;
  unsigned int ac_msbits2 = 8;
  unsigned int ac_msbits3 = 0;
  unsigned int ac_msbits4 = 0; // cooling | 4:heat
  unsigned int ac_msbits5 =  (temperature < 15) ? 0 : temperature - 15;
  unsigned int ac_msbits6 = 0;

  if (air_flow <= 2) {
    if (kAc_Type == 0)
      ac_msbits6 = kAc_Flow_Tower[air_flow];
    else
      ac_msbits6 = kAc_Flow_Wall[air_flow];
  }

  // calculating using other values
  unsigned int ac_msbits7 = (ac_msbits3 + ac_msbits4 + ac_msbits5 + ac_msbits6) & B00001111;
  ac_code_to_sent = ac_msbits1 << 4;
  ac_code_to_sent = (ac_code_to_sent + ac_msbits2) << 4;
  ac_code_to_sent = (ac_code_to_sent + ac_msbits3) << 4;
  ac_code_to_sent = (ac_code_to_sent + ac_msbits4) << 4;
  ac_code_to_sent = (ac_code_to_sent + ac_msbits5) << 4;
  ac_code_to_sent = (ac_code_to_sent + ac_msbits6) << 4;
  ac_code_to_sent = (ac_code_to_sent + ac_msbits7);

  irsend.sendLG(ac_code_to_sent, 28);

}

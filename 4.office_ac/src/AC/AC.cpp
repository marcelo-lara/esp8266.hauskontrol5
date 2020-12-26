#include "../Core/Devices/AC/AC.h"

// git@github.com:crankyoldgit/IRremoteESP8266.git
#include <IRremoteESP8266.h>
#include <IRsend.h>
IRsend irsend(14);  // An IR LED is controlled by GPIO pin 14 (D5)

const unsigned int kAc_Type = 1; // 1: Wall
unsigned int        ac_heat = 0; // 0: Cool | 1: Heat
unsigned int    ac_power_on = 1; // 0: Off  | 1: On
unsigned int ac_air_clean_state = 0; // 0 : off | 1 : on --> power on
unsigned int ac_temperature = 24; // 18 ~ 30
unsigned int ac_flow = 0; // 0: low | 2: high
const uint8_t kAc_Flow_Tower[3] = {0, 4, 6};
const uint8_t kAc_Flow_Wall[4] = {0, 2, 4, 5};    

//private
void Ac_Activate(unsigned int temperature, unsigned int air_flow, unsigned int heat);
void Ac_Send_Code(uint32_t code);


AC::AC(int _irOut, int defaultTemp) 
{
    
}

void AC::init(){
    Serial.println("-- AC::init() --");
    irsend.begin();
}

void AC::turnOn() 
{
    Serial.println("-- AC::turnOn() --");
    Ac_Activate(23, 2, 0);

    this->isOn=true;
}

void AC::turnOff() 
{
    irsend.sendLG(0x88C0051, 28);
    this->isOn=false;
}

void AC::toggle() 
{
    this->isOn=!(this->isOn);
}

void AC::setTemp(int _temp) 
{
    
}

uint32_t ac_code_to_sent;

void Ac_Send_Code(uint32_t code) {
  Serial.print("code to send : ");
  Serial.print(code, BIN);
  Serial.print(" : ");
  Serial.println(code, HEX);

  irsend.sendLG(code, 28);
}

void Ac_Activate(unsigned int temperature, unsigned int air_flow,
                 unsigned int heat) {
  Serial.println("-- Ac_Activate");
  ac_heat = heat;
  unsigned int ac_msbits1 = 8;
  unsigned int ac_msbits2 = 8;
  unsigned int ac_msbits3 = 0;
  unsigned int ac_msbits4;
  if (ac_heat == 1)
    ac_msbits4 = 4;  // heating
  else
    ac_msbits4 = 0;  // cooling
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

void Ac_Power_Down() {
  ac_code_to_sent = 0x88C0051;

  Ac_Send_Code(ac_code_to_sent);

  ac_power_on = 0;
}

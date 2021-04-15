#include "WebApi.h"

WebApi::WebApi(Controller _controller){
    this->controller=_controller;
    switch (this->controller){
        case Living: this->node_name="Living";break;
        case Office: this->node_name="Office";break;
        case OfficeAc: this->node_name="OfficeAc";break;
        case Stage3: this->node_name="Stage 3";break;
    }
};

void WebApi::set_devices(Device** _dev_list, int _dev_count){
    this->device_list = _dev_list;
    this->device_count = _dev_count;
};

void WebApi::setup(){
    this->setup_web();
    this->setup_mqtt();
};

void WebApi::update(){
  this->server->handleClient();

  if (!this->mqtt->connected()) {this->mqtt_connect();};
  this->mqtt->loop();

};

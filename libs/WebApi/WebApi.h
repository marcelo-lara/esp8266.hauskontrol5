#pragma once
#include "../Core/Devices/Device.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <uri/UriRegex.h>

//device types
#include "../Core/Devices/Environment/Environment.h"
#include "../Core/Devices/Light/Light.h"
#include "../Core/Devices/Fan/Fan.h"
#include "../Core/Devices/AC/AC.h"

class WebApi {
public:
    enum Controller {Living, Office, Suite, OfficeAc, Stage3};
    Controller controller;
    String node_name;

    WebApi(Controller _controller);
    void setup();
    void update();

    //devices
    Device** device_list;
    int      device_count;
    void set_devices(Device** _dev_list, int dev_count);
    void list_devices();


    //web ui
    ESP8266WebServer *server;
    void setup_web();

private:
    //web ui
    void web_send_root();
    String html_dev(Device* dev);

    //api
    void json_send_status();
    void send_result(String result_str);


};
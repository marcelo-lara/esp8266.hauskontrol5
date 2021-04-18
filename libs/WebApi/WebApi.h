#pragma once

//web related
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <uri/UriRegex.h>

//mqtt
#include <PubSubClient.h>

//device types
#include "../Core/Devices/Device.h"
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

    //web ui
    ESP8266WebServer *server;
    void setup_web();

    //mqtt
    PubSubClient* mqtt;
    void setup_mqtt();
    void mqtt_connect();
    void mqtt_publish(const char* topic, const char* message);
    void mqtt_publish(Device* dev);
    void mqtt_callback(char* topic, byte* payload, unsigned int length);
    
    long mqtt_retry_time;
    
    String mqtt_controller_topic;

private:
    //web ui
    void web_send_root();
    String html_dev(Device* dev);

    //api
    void json_send_status();
    void send_result(String result_str);


};
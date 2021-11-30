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

typedef void MqttTopicReceivedCB(String topic, String payload);

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
    void mqtt_publish(const char* _topic, const char* _message);
    void mqtt_publish(Device* dev);
    void mqtt_publish(Fan* fan);
    void mqtt_publish(Environment* env);
    void mqtt_subscribe(const char* topic);
    void mqtt_handle_callback(Fan* fan, String cmd, String payload);
    String mqtt_controller_topic;
    MqttTopicReceivedCB* mqttTopicReceivedCb; 

private:
    //web ui
    void web_send_root();
    String html_dev(Device* dev);
    String html_button(String classname, String target, String caption);
    String html_button(String classname, String target, String caption, bool state);

    //api
    void json_send_status();
    void send_result(String result_str);

    //mqtt
    void mqtt_callback(char* topic, byte* payload, unsigned int length);
    long mqtt_retry_time;

};
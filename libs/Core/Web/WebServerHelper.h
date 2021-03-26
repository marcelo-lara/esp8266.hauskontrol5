#pragma once
#include <Arduino.h>
#include "../Devices/Light/Light.h"
#include <ESP8266WebServer.h>   // Include the WebServer library

class WebServerHelper {
public:
    ESP8266WebServer *_server;

    WebServerHelper(ESP8266WebServer *server){
        this->_server = server;
    };

    ESP8266WebServer *get_server(){
        return this->_server;
    };

    void status_pattern(bool state){
        //response body
        String r_body;
        r_body += "{\"statusPattern\":\"";
        r_body += state?"true":"false";
        r_body += "\"}";

        //deliver message
        this->_server->send(200, "application/json", r_body);
    };

    void send_root(String html_devices, String node_name){
        String r_body;
        r_body += "<html>";
        r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta http-equiv=\"refresh\" content=\"60\"/><style>";
        r_body += "html,body,h2{background-color:#000;color:#eee;font-family:sans-serif;margin:0;padding:0}";
        r_body += "section,h1,h2{padding:15px}";
        r_body += "div.block{width:8em;height:8em;display:inline-block;padding:1em;background-color:#222;margin:.25em;cursor:pointer}";
        r_body += "h1{background-color:#111;color:#ccc}";
        r_body += "div.button.on{background-color:#282}";
        r_body += "</style></head><body>";
        r_body += "<h1>" + node_name + "</h1><section>";
        r_body += html_devices;
        r_body += "</section><script>for(b of document.querySelectorAll(\"div.button\")){let t=b.getAttribute(\"target\");b.addEventListener(\"click\", ()=>{fetch(t).then(()=>{document.location.reload(true)})})}</script>";
        r_body += ("</body></html>");
        //send
        this->_server->send(200, "text/html", r_body);
    }

    void send_result(String result_str){
        this->_server->sendHeader("Access-Control-Allow-Origin", "*");
        this->_server->send(200, "application/json", "{\"result\":\"" + result_str + "\"}"); // Send 
    };

    void send_status(String json_dev_list){
        String r_body;
        r_body += "{";
        r_body += json_dev_list;
        r_body += "}";

        this->_server->sendHeader("Access-Control-Allow-Origin", "*");
        this->_server->send(200, "application/json", r_body); // Send 
    };

    void init(){

        //set CORS options
        this->_server->onNotFound([&](){
            if (this->_server->method() == HTTP_OPTIONS){
                this->_server->sendHeader("Access-Control-Allow-Origin", "*");
                this->_server->sendHeader("Access-Control-Max-Age", "10000");
                this->_server->sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
                this->_server->sendHeader("Access-Control-Allow-Headers", "*");
                this->_server->send(204);
            }else{
                this->_server->send(404, "text/plain", "");
            }
        });  

        //start server
        this->_server->begin();
    };

    // device apis ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //listen to light requests
    void add_light(Light *light){
        String baseUri="/set/light/" + String(light->name);
        this->_server->on(String(baseUri + "/on"),     [light, this]() { light->turnOn();  return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/off"),    [light, this]() { light->turnOff(); return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/toggle"), [light, this]() { light->toggle();  return this->send_result("ok"); });
        this->_server->on(String("/sp/light/" + String(light->name)), [light, this]() { return this->status_pattern(light->isOn); });
    }

    // helpers ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    String json_obj_block(String block_name, String block_content){
        String json_res;
        json_res = "\"" + block_name + "\":{";
        json_res += block_content;
        json_res += "}";
        return json_res;
    }


};

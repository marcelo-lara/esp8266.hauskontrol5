#pragma once
#include <Arduino.h>
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

    void send_root(String html_devices){
        String r_body;
        r_body += "<html>";
        r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>";
        r_body += html_devices;
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


    // void server_begin(){
    //     &_server->onNotFound([](){
    //         if (&_server->method() == HTTP_OPTIONS){
    //             &_server->sendHeader("Access-Control-Allow-Origin", "*");
    //             &_server->sendHeader("Access-Control-Max-Age", "10000");
    //             &_server->sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    //             &_server->sendHeader("Access-Control-Allow-Headers", "*");
    //             &_server->send(204);
    //         }else{
    //             &_server->send(404, "text/plain", "");
    //         }
    //     });  
    //     &_server->begin();
    // }

};

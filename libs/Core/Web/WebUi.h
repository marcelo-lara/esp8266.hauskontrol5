#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <uri/UriRegex.h>


#include "../Devices/Environment/Environment.h"
#include "../Devices/Light/Light.h"
#include "../Devices/Fan/Fan.h"
#include "../Devices/AC/AC.h"
ESP8266WebServer __server(80);

enum Controller{
    Suite,
    Office,
    OfficeAc,
    Living,
    Stage3
};

class WebUi {
public:
    ESP8266WebServer *_server;
    String _node_name;
    Controller _node;


    //attached devices
    AC *_ac;
    Fan *_fan;
    Light *_light;
    Environment *_environment;

    WebUi(Controller __node, String node_name){
        this->_node = __node;
        this->_node_name=node_name;
        this->_server = &__server;
    };

    WebUi(String node_name){
        this->_node = Controller::OfficeAc;
        this->_node_name=node_name;
        this->_server = &__server;
    };

    WebUi(ESP8266WebServer *server, String node_name){
        this->_node_name=node_name;
        this->_server = server;
    };

    ESP8266WebServer *get_server(){
        return this->_server;
    };

    void status_pattern(bool state){
        String r_body;
        r_body = "{\"statusPattern\":\"" + String(state?"true":"false") +"\"}";
        this->_server->sendHeader("Access-Control-Allow-Origin", "*");
        this->_server->send(200, "application/json", r_body);
    };

    void send_result(String result_str){
        this->_server->sendHeader("Access-Control-Allow-Origin", "*");
        this->_server->send(200, "application/json", "{\"result\":\"" + result_str + "\"}"); // Send 
    };

    void send_status(String json_dev_list){
        String r_body;
        r_body += "{\"name\":\"" +  String(this->_node_name) + "\"," ;
        r_body += json_dev_list;
        r_body += "}";

        this->_server->sendHeader("Access-Control-Allow-Origin", "*");
        this->_server->send(200, "application/json", r_body); // Send 
    };

    void init(){

        this->_server->on("/",       [this]() { this->html_ui();     });
        this->_server->on("/status", [this]() { this->json_status(); });


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

    void update(){
        this->_server->handleClient();
    }

    // device apis ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // lights
    void add_device(Light *light){
        this->_light = light;
        String baseUri="/set/light/" + String(light->name);
        this->_server->on(String(baseUri + "/on"),     [light, this]() { light->turnOn();  return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/off"),    [light, this]() { light->turnOff(); return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/toggle"), [light, this]() { light->toggle();  return this->send_result("ok"); });
        this->_server->on(String("/sp/light/" + String(light->name)), [light, this]() { return this->status_pattern(light->isOn); });
    };

    // fan
    void add_device(Fan *fan){
        this->_fan=fan;
        String baseUri="/set/fan";
        this->_server->on(String(baseUri + "/on"),      [fan, this]() { fan->turnOn();  return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/off"),     [fan, this]() { fan->turnOff();  return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/toggle"),  [fan, this]() { fan->toggle();  return this->send_result("ok"); });
        for (int i = 0; i < fan->max_speed; i++){
            this->_server->on(String(baseUri + "/speed/" + String(i+1)),      [fan, this, i]() { fan->setSpeed(i+1);  return this->send_result("ok"); });
        };
        this->_server->on("/sp/fan",      [fan, this]() { return this->status_pattern(fan->isOn); });
    };

    // ac
    void add_device(AC *ac){
        this->_ac=ac;
        String baseUri="/set/ac";
        this->_server->on(String(baseUri + "/on"),      [ac, this]() { ac->turnOn();     return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/off"),     [ac, this]() { ac->turnOff();    return this->send_result("ok"); });
        this->_server->on(String(baseUri + "/toggle"),     [ac, this]() { ac->toggle();  return this->send_result("ok"); });
        this->_server->on("/sp/ac", [ac, this]() { return this->status_pattern(ac->isOn); });

        //todo: improve me
        this->_server->on("/set/ac/temp/19",  [ac, this]() { ac->setTemp(19); return this->send_result("ok"); });
        this->_server->on("/set/ac/temp/20",  [ac, this]() { ac->setTemp(20); return this->send_result("ok"); });
        this->_server->on("/set/ac/temp/21",  [ac, this]() { ac->setTemp(21); return this->send_result("ok"); });
        this->_server->on("/set/ac/temp/22",  [ac, this]() { ac->setTemp(22); return this->send_result("ok"); });
        this->_server->on("/set/ac/temp/23",  [ac, this]() { ac->setTemp(23); return this->send_result("ok"); });
        this->_server->on("/set/ac/temp/24",  [ac, this]() { ac->setTemp(24); return this->send_result("ok"); });
        this->_server->on("/set/ac/temp/25",  [ac, this]() { ac->setTemp(25); return this->send_result("ok"); });
        this->_server->on("/set/ac/flow/0",   [ac, this]() { ac->setFlow(0);  return this->send_result("ok"); });
        this->_server->on("/set/ac/flow/1",   [ac, this]() { ac->setFlow(1);  return this->send_result("ok"); });
        this->_server->on("/set/ac/flow/2",   [ac, this]() { ac->setFlow(2);  return this->send_result("ok"); });
        this->_server->on("/set/ac/swing/on", [ac, this]() { ac->swingOn();   return this->send_result("ok"); });
        this->_server->on("/set/ac/swing/off",[ac, this]() { ac->swingOff();  return this->send_result("ok"); });
    };

    void add_device(Environment *environment){
        this->_environment=environment;
    }

    // json status ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    String _json_status(AC *ac){
        return String("\"ac\":{\"power\":\"" + String(ac->isOn)  + "\", \"temp\":\""  + String(ac->temp)  + "\",\"flow\":\""  + String(ac->flow)  + "\",\"swing\":\"" + String(ac->swing) + "\"}");
    };

    String _json_status(Environment *env){
        return "\"env\":{\"temp\":\""  + String(env->temperature) + "\",\"hum\":\""   + String(env->humidity) + "\",\"pres\":\""  + String(env->pressure) + "\",\"light\":\"" + String(env->light) + "\"}";
    };

    String _json_status(Fan *fan){
        return "\"fan\":{\"speed\":\"" + String(fan->speed) + "\"}";
    };

    String _json_status(Light *light){
        return "\"lights\":[{\"" + String(light->name) + "\":\"" + String(light->isOn?"1":"0") + "\"}]";
    };

    void json_status(){
        String r_body;
        r_body = "{\"name\":\"" +  String(this->_node_name) + "\"," ;

        switch (this->_node){

        case Controller::Suite :
           r_body += this->_json_status(this->_light) + ",";
           r_body += this->_json_status(this->_fan);
            break;

        case Controller::Office :
            r_body += this->_json_status(this->_light) + ",";
            r_body += this->_json_status(this->_environment);
            break;

        case Controller::OfficeAc :
            r_body += this->_json_status(this->_ac) + ",";
            r_body += this->_json_status(this->_environment);
            break;

        case Controller::Stage3 :
            r_body += this->_json_status(this->_light) + ",";
            r_body += this->_json_status(this->_fan);
            break;

        };

        r_body += "}";

        //send
        this->_server->sendHeader("Access-Control-Allow-Origin", "*");
        this->_server->send(200, "application/json", r_body); // Send 
    };

    // json helpers 
    String json_obj_block(String block_name, String block_content){
        String json_res;
        json_res = "\"" + block_name + "\":{";
        json_res += block_content;
        json_res += "}";
        return json_res;
    };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/// Devices ///
    String _get_html(AC *ac){
      String dev_html;
      dev_html += "<div class=\"block button ac ";
      dev_html += String(ac->isOn?"on":"off");
      dev_html += "\"";
      dev_html += "target=\"/set/ac/" + String(ac->isOn?"off":"on") + "\">";
      dev_html += String(ac->isOn?"on":"off");
      dev_html += "</div>";

      //temp
      dev_html += "<div class=\"fb\"><h3>temp</h3>";
      for (int i = 20; i < 25; i++){
        dev_html += "<div class=\"block button ac " + String(ac->temp==i?"on":"") + "\" ";
        dev_html += "target=\"/set/ac/temp/" + String(i) + "\">";
        dev_html += String(i);
        dev_html += "</div>";
      }
      dev_html += "</div>";

      //flow
      dev_html += "<div class=\"fb\"><h3>flow</h3>";
      for (int i = 0; i < 5; i++){
        dev_html += "<div class=\"block button acflow " + String(ac->flow==i?"on":"") + "\" ";
        dev_html += "target=\"/set/ac/flow/" + String(i) + "\">";
        dev_html += String(i);
        dev_html += "</div>";
      }
      dev_html += "</div>";


      //swing
      dev_html += "<div class=\"fb\"><h3>swing</h3>";
      dev_html += "<div class=\"block button acswing " + String(ac->swing?"on":"off") + "\" ";
      dev_html += "target=\"/set/ac/swing/" + String(ac->swing?"off":"on") + "\">";
      dev_html += String(ac->swing?"on":"off");
      dev_html += "</div>";
      dev_html += "</div>";
      return dev_html;
    };



    void html_ui(){
        String dev_html;

        switch (this->_node){

        case Controller::Suite :
            dev_html += "<h2>lights</h2>";
            dev_html += this->_light->to_html_div();
            dev_html += "<h2>fan</h2>";
            dev_html += this->_fan->to_html();
            break;

        case Controller::OfficeAc :
            dev_html += "<h2>environment</h2>";
            dev_html += this->_environment->to_html();
            dev_html += "<h2>ac</h2>";
            dev_html += this->_get_html(this->_ac);
            break;

        case Controller::Office :
            dev_html += "<h2>lights</h2>";
            dev_html += this->_light->to_html_div();
            dev_html += "<h2>environment</h2>";
            dev_html += this->_environment->to_html();
            break;

        case Controller::Stage3 :
            dev_html += "<h2>lights</h2>";
            dev_html += this->_light->to_html_div();
            dev_html += "<h2>fan</h2>";
            dev_html += this->_fan->to_html();
            break;

        default:
            dev_html += "<h2>no devices??</h2>";
            break;
        }

        this->send_root(dev_html);

    };

    void send_root(String html_devices){
        String r_body;
        r_body += "<html>";
        r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta http-equiv=\"refresh\" content=\"60\"/><style>";
        r_body += "html,body,h2{background-color:#000;color:#eee;font-family:sans-serif;margin:0;padding:0}";
        r_body += "section,h1,h2{padding:15px}";
        r_body += "div.block{width:8em;height:8em;display:inline-block;padding:1em;background-color:#222;margin:.25em;cursor:pointer}";
        r_body += "h1{background-color:#111;color:#ccc}";
        r_body += "div.button.on{background-color:#282}";
        r_body += "h3{writing-mode: vertical-rl;display: inline-block;width: 1em; height: 8em;transform: rotate(180deg);}";
        r_body += ".fb{display: flex;}";
        r_body += ".fb>div{flex: 0 0 auto;}";
        r_body += "</style></head><body>";
        r_body += "<h1>" + String(this->_node_name) + "</h1><section>";
        r_body += html_devices;
        r_body += "</section><script>for(b of document.querySelectorAll(\"div.button\")){let t=b.getAttribute(\"target\");b.addEventListener(\"click\", ()=>{fetch(t).then(()=>{document.location.reload(true)})})}</script>";
        r_body += ("</body></html>");
        //send
        this->_server->send(200, "text/html", r_body);
    }



};

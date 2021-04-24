#include "WebApi.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WEB BLOCKS

void WebApi::setup_web(){
    this->server = new ESP8266WebServer(80);

    this->server->on("/",       [this]() { this->web_send_root();     });
    this->server->on("/status", [this]() { this->json_send_status();  });

    //devices
    for (int i = 0; i < this->device_count; i++){
        Device* dev = this->device_list[i];
        switch (dev->type){

        case Device::DevType_e::Light:{
            this->server->on(String("/set/light/" + String(dev->name) + "/toggle"), [this, dev]() { dev->toggle();  return this->send_result("ok"); });
            this->server->on(String("/set/light/" + String(dev->name) + "/on"),     [this, dev]() { dev->turnOn();  return this->send_result("ok"); });
            this->server->on(String("/set/light/" + String(dev->name) + "/off"),    [this, dev]() { dev->turnOff(); return this->send_result("ok"); });
            break;
        }

        case Device::DevType_e::Fan:{
            this->server->on(String("/set/fan/toggle"), [this, dev]() { dev->toggle();  return this->send_result("ok"); });
            this->server->on(String("/set/fan/on"),     [this, dev]() { dev->turnOn();  return this->send_result("ok"); });
            this->server->on(String("/set/fan/off"),    [this, dev]() { dev->turnOff(); return this->send_result("ok"); });
            for (int i = 0; i < ((Fan*)dev)->max_speed+1; i++){
                this->server->on(String("/set/fan/speed/"+ String(i)),    [this, dev, i]() {  ((Fan*)dev)->setSpeed(i); return this->send_result("ok"); });
            };
            break;
        }

        };
    };


    //set CORS options
    this->server->onNotFound([&](){
        if (this->server->method() == HTTP_OPTIONS){
            this->server->sendHeader("Access-Control-Allow-Origin", "*");
            this->server->sendHeader("Access-Control-Max-Age", "10000");
            this->server->sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
            this->server->sendHeader("Access-Control-Allow-Headers", "*");
            this->server->send(204);
        }else{
            this->server->send(404, "text/plain", "");
        }
    });  

    //start server
    this->server->begin();

};

void WebApi::web_send_root(){
    String r_body;
    r_body += "<html>";
    r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta http-equiv=\"refresh\" content=\"60\"/><style>";
    r_body += "html,body,h2{background-color:#000;color:#eee;font-family:sans-serif;margin:0;padding:0}";
    r_body += "section,h1,h2{padding:15px}";
    r_body += "div.block{width:8em;height:8em;display:inline-grid;padding:1em;background-color:#222;margin:.25em;cursor:pointer}";
    r_body += "h1{background-color:#111;color:#ccc}";
    r_body += "div.button.on{background-color:#282}";
    r_body += "h3{writing-mode: vertical-rl;display: inline-block;width: 1em; height: 8em;transform: rotate(180deg);}";
    r_body += ".fb{display: flex;}";
    r_body += ".fb>div{flex: 0 0 auto;}";
    r_body += "</style></head><body>";
    r_body += "<h1>" + String(this->node_name) + "</h1><section>";

    //devices
    for (int i = 0; i < this->device_count; i++){r_body += String(this->html_dev(this->device_list[i]));};
    r_body += "</section><script>for(b of document.querySelectorAll(\"div.button\")){let t=b.getAttribute(\"target\");b.addEventListener(\"click\", ()=>{fetch(t).then(()=>{document.location.reload(true)})})}</script>";
    r_body += ("</body></html>");
    
    //send
    this->server->send(200, "text/html", r_body);
};

String WebApi::html_dev(Device* dev){
    String dev_html;
    switch (dev->type){
    
    case Device::DevType_e::Light:{
        dev_html += "<div";
        dev_html += " class=\"block button light " + String(dev->isOn?"on":"off") + "\"";
        dev_html += " target=\"set/light/" + dev->name + "/toggle\"";
        dev_html += ">";
        dev_html += String(dev->name);
        dev_html += "</div>";
        break;
    }
    
    case Device::DevType_e::Fan:{
        Fan* fan = (Fan*)dev;
        for (int i = 0; i < fan->max_speed+1; i++){
            dev_html += "<div";
            if(i==0){
                dev_html += " class=\"block button fan " + String(fan->isOn?"on":"off") + "\"";
                dev_html += " target=\"set/fan/" + String(fan->isOn?"off":"on")  + "\"";
                dev_html += ">";
                dev_html += "fan";
            }else{
                dev_html += " class=\"block button fan " + String(i==fan->speed?"on":"") + "\"";
                dev_html += " target=\"set/fan/speed/" + String(i) + "\"";
                dev_html += ">";
                dev_html += String(i);
            };
            dev_html += "</div>";
        };
        break;
    }

    default:{
        dev_html += "<div class=\"block " + String(dev->isOn?"on":"off") + "\">";
        dev_html += "unhandled: " + String(dev->name);
        dev_html += "</div>";
        break;
        }
    };
    return dev_html;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JSON api
void WebApi::json_send_status(){
    String r_body;
    r_body += "{\"name\":\"" +  String(this->node_name) + "\"," ;

    // lights array
    r_body += "\"lights\":[";
    for (int i = 0; i < this->device_count; i++){
        if(!this->device_list[i]->type == Device::DevType_e::Light){
            r_body += "{\"" + String(device_list[i]->name) + "\":\"" + String(device_list[i]->isOn?"1":"0") + "\"},";
        };
    };
    r_body = r_body.substring(0, r_body.length()-1);
    r_body += "]";

    // devices
    for (int i = 0; i < this->device_count; i++){
        if(this->device_list[i]->type==Device::DevType_e::Light) continue;
        switch (device_list[i]->type){
        
        case Device::DevType_e::Fan:{ //"fan":{"speed":"4"}
            r_body += ",\"fan\":{\"speed\":\"" + String(((Fan*)device_list[i])->speed) + "\"}";
        }

        };

    }
    

    r_body += "}";

    // send json
    this->server->sendHeader("Access-Control-Allow-Origin", "*");
    this->server->send(200, "application/json", r_body); // Send 
};

void WebApi::send_result(String result_str){
    this->server->sendHeader("Access-Control-Allow-Origin", "*");
    this->server->send(200, "application/json", "{\"result\":\"" + result_str + "\"}"); // Send 
};
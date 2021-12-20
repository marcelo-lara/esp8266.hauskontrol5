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


        case Device::DevType_e::AC:{
            this->server->on(String("/set/ac/on"),        [this, dev]() { dev->turnOn();  return this->send_result("ok"); });
            this->server->on(String("/set/ac/off"),       [this, dev]() { dev->turnOff(); return this->send_result("ok"); });
            this->server->on(String("/set/ac/swing/on"),  [this, dev]() { ((AC*)dev)->swingOn();  return this->send_result("ok"); });
            this->server->on(String("/set/ac/swing/off"), [this, dev]() { ((AC*)dev)->swingOff(); return this->send_result("ok"); });
            this->server->on(String("/set/ac/flow/0"),    [this, dev]() { ((AC*)dev)->setFlow(0);  return this->send_result("ok"); });
            this->server->on(String("/set/ac/flow/1"),    [this, dev]() { ((AC*)dev)->setFlow(1);  return this->send_result("ok"); });
            this->server->on(String("/set/ac/flow/2"),    [this, dev]() { ((AC*)dev)->setFlow(2);  return this->send_result("ok"); });
            for (int i = ((AC*)dev)->min_temp; i < ((AC*)dev)->max_temp+1; i++){
                this->server->on(String("/set/ac/temp/"+ String(i)),    [this, dev, i]() {  ((AC*)dev)->setTemp(i); return this->send_result("ok"); });
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
    //minified CSS
    r_body += "html,body,h2{background-color:#000;color:#eee;font-family:sans-serif;margin:0;padding:0}section,h1{padding:15px}h1{padding:15px}h2{transform:rotate(270deg);display:inline-block;width:2em;height:1em}div.block{width:8em;height:8em;display:inline-grid;padding:1em;background-color:#222;margin:.25em;cursor:pointer;position:relative}div>input[type='range']{position:absolute;bottom:.25em;width:100%;height:2em}h1{background-color:#111;color:#ccc}div.on{background-color:#282}h3{writing-mode:vertical-rl;display:inline-block;width:1em;height:8em;transform:rotate(180deg)}.fb{display:flex}.fb>div{flex:0 0 auto}";

    r_body += "</style></head><body>";
    r_body += "<h1>" + String(this->node_name) + "</h1><section>";

    //devices
    for (int i = 0; i < this->device_count; i++){r_body += String(this->html_dev(this->device_list[i]));};
    r_body += "</section>";

    // JS
    r_body += "<script>";
    r_body += "api=t=>fetch(t).then(()=>{document.location.reload(true)});";
    r_body += "for(b of document.querySelectorAll(\"input[target]\"))b.addEventListener(\"change\",e=>api(`${e.target.getAttribute(\"target\")}/${e.target.value}`));";
    r_body += "for(b of document.querySelectorAll(\"div.button[target]\"))b.addEventListener(\"click\",e=>api(e.target.getAttribute(\"target\")));";
    r_body += "</script>";


    r_body += ("</body></html>");
    
    //send
    this->server->send(200, "text/html", r_body);
};

String WebApi::html_button(String classname, String target, String caption){
    return  "<div"
            " class=\"block button " + classname + "\""
         +  " target=\"" + target  + "\""
         + ">" + caption + "</div>";
}

String WebApi::html_button(String classname, String target, String caption, bool state){
    return  "<div"
            " class=\"block button " + String(state?"on":"off") + " " + classname + "\""
         +  " target=\"" + target  + String(state?"off":"on") + "\""
         + ">" + caption + " " + String(state?"on":"off") + "</div>";
}

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

    case Device::DevType_e::Environment:{
        Environment* env = (Environment*)dev;
        dev_html += "<section><h2>Env</h2>";
        dev_html += "<div class=\"block env temp\">temp "         + String(env->temperature) + "</div>";
        dev_html += "<div class=\"block env humidity\">humidity " + String(env->humidity) + "</div>";
        dev_html += "<div class=\"block env pressure\">pressure " + String(env->pressure) + "</div>";
        dev_html += "<div class=\"block env light\">light "       + String(env->illuminance) + "</div>";
        dev_html += "</section>";
        break;
    }

    case Device::DevType_e::AC:{
        AC* ac = (AC*)dev;
        dev_html += "<section><h2>AC</h2>";

        //power
        dev_html += html_button("ac", "set/ac/", "pwr", ac->isOn);
        
        //temp
        dev_html += "<div class=\"block ac temp\">temp: <span>"  + String(ac->temp) + "</span>";
        dev_html +=   "<input type=\"range\" min=\"" + String(ac->min_temp) + "\" max=\"" + String(ac->max_temp) + "\" step=\"1\" value=\"" + String(ac->temp) + "\" target=\"set/ac/temp\">";
        dev_html += "</div>";

        //flow
        dev_html += "<div class=\"block ac flow\">flow: <span>" + String(ac->flow) + "</span>";
        dev_html +=   "<input type=\"range\" min=\"0\" max=\"2\" step=\"1\" value=\"" + String(ac->flow) + "\" target=\"set/ac/flow\">";
        //dev_html +=   "<input type=\"range\" min=\"" + String(ac->min_flow) + " \" max=\"" + String(ac->max_flow) + "\" step=\"1\" value=\"" + String(ac->temp) + "\" target=\"set/temp\">";
        dev_html += "</div>";

        //swing
        dev_html += html_button("swing", "set/ac/swing/", "swing", ac->swing);
        dev_html += "</section>";

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
    bool has_lights = false;
    r_body += "{\"name\":\"" +  String(this->node_name) + "\"," ;

    // lights array
    r_body += "\"lights\":[";
    for (int i = 0; i < this->device_count; i++){
        if(!this->device_list[i]->type == Device::DevType_e::Light){
            r_body += "{\"" + String(device_list[i]->name) + "\":\"" + String(device_list[i]->isOn?"1":"0") + "\"},";
            has_lights = true;
        };
    };
    if(has_lights) r_body = r_body.substring(0, r_body.length()-1);
    r_body += "]";

    // devices
    for (int i = 0; i < this->device_count; i++){
        if(this->device_list[i]->type==Device::DevType_e::Light) continue;
        switch (device_list[i]->type){
            
            case Device::DevType_e::Fan:{ //"fan":{"speed":"4"}
                r_body += ",\"fan\":{\"speed\":\"" + String(((Fan*)device_list[i])->speed) + "\"}";
            }

            case Device::DevType_e::Environment:{ //"fan":{"speed":"4"}
                r_body += ",\"environment\":{\"temperature\":\""  + String(((Environment*)device_list[i])->temperature) + "\",\"humidity\":\""   + String(((Environment*)device_list[i])->humidity) + "\",\"pressure\":\""  + String(((Environment*)device_list[i])->pressure) + "\", \"illuminance\":\"" + String(((Environment*)device_list[i])->illuminance) + "\"}";
            };
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
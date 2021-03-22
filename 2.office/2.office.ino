#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <uri/UriRegex.h>
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define wallSwitchPin  14 // D5 Wall Switch
#define statusLedPin   12 // D6 Wall StatusLed
#define relayOutPin    13 // D7 relay out
#define irSendPin       0 // D3 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led

ESP8266WebServer server(80);    // WebServer object

//// devices ////
#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(relayOutPin, true);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;


void setup() {
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);

  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=[](int cc) {light.toggle();};
  light.turnOn();

  //connect
  wemosWiFi.connect("office");
  light.turnOff();

  // Start the server
  server_setup();
 
  //non-critical hardware
  environment.setup();
  digitalWrite(statusLedPin, HIGH);

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  wallSwitch.update();
  environment.update();

  //
  wemosWiFi.update();
  server.handleClient();

}

void server_setup(){
  // node
  server.on("/",                []() { server.send(200, "text/html", ui_root()); return; });
  server.on("/status",          []() { 
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", status_json()); return; }
  );

  // light
  server.on("/set/light/on",       []() { light.turnOn();    return return_result(); });
  server.on("/set/light/off",      []() { light.turnOff();   return return_result(); });
  server.on("/set/light/toggle",   []() { light.toggle();   return return_result(); });
  server.on("/status/light",       []() { return server.send(200, "application/json", status_pattern(light.isOn)); });

  //server
  server.onNotFound([](){
    if (server.method() == HTTP_OPTIONS){
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Max-Age", "10000");
        server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.send(204);
    }else{
        server.send(404, "text/plain", "");
    }
  });  
  server.begin();
}

String ui_root(){
  String r_body;
  r_body += "<html>";
  r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>";

  //light
  r_body += "<h2>light</h2>";
  r_body += "<span>";
  r_body += light.isOn?"On":"Off";
  r_body += "</span>";
  r_body += "<a href=\"/set/light/toggle\" target=\"i_result\"><button>switch</button></a><br>";

  r_body += ("</body></html>");
  return r_body;
}

String status_json(){
  String r_body;
  r_body += "{";
  r_body += light.to_json() +",";
  r_body += "\"env\":" + environment.to_json();
  r_body += "}";
  return r_body;
}

String status_pattern(bool state){
  String r_body;
  r_body += "{\"statusPattern\":\"";
  r_body += state?"true":"false";
  r_body += "\"}";
  return r_body;
}

void return_result(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"result\":\"ok\"}"); // Send 
}


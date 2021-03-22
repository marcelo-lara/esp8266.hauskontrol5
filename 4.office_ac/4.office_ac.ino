#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <uri/UriRegex.h>
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define irSendPin      14 // D5 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led
#define acOnSwitchPin  12 // D6 pullup 
#define acOnLedPin     13 // D7

ESP8266WebServer server(80);    // WebServer object

//// devices ////
#include "src/Core/Devices/AC/AC.h"
AC ac(irSendPin, 24);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;

void setup() {

  //core devices
  ac.init();

  //connect
  wemosWiFi.connect("officeac");

  // web ui
  server.on("/",                []() { server.send(200, "text/html", ui_root()); return; });
  server.on("/status",          []() { 
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", status_json()); return; });
  server.on("/get/ac/on",       []() { server.send(200, "application/json", ac_status_json()); return; });

  // ac settings
  server.on("/set/ac/on",       []() { ac.turnOn();    return return_result(); });
  server.on("/set/ac/off",      []() { ac.turnOff();   return return_result(); });

  server.on("/set/ac/temp/21",  []() { ac.setTemp(21); return return_result(); });
  server.on("/set/ac/temp/22",  []() { ac.setTemp(22); return return_result(); });
  server.on("/set/ac/temp/23",  []() { ac.setTemp(23); return return_result(); });
  server.on("/set/ac/temp/24",  []() { ac.setTemp(24); return return_result(); });
  server.on("/set/ac/temp/25",  []() { ac.setTemp(25); return return_result(); });

  server.on("/set/ac/flow/0",   []() { ac.setFlow(0);  return return_result(); });
  server.on("/set/ac/flow/1",   []() { ac.setFlow(1);  return return_result(); });
  server.on("/set/ac/flow/2",   []() { ac.setFlow(2);  return return_result(); });

  server.on("/set/ac/swing/on", []() { ac.swingOn();   return return_result(); });
  server.on("/set/ac/swing/off",[]() { ac.swingOff();  return return_result(); });

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
  
  //non-critical hardware
  environment.setup();

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  pinMode(acOnLedPin, OUTPUT);
  digitalWrite(acOnLedPin, HIGH);


}

void loop() {
  wemosWiFi.update();

  environment.update();
  server.handleClient();

}

void return_result(){
  server.send(200, "text/plain", "OK"); // Send 
}

String ui_root(){
  String r_body;
  r_body += "<html>";
  r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>";

//ac
  r_body += "<h2>AC</h2> ";
  r_body += "<p>status: ";
  r_body += ac.isOn?"On":"Off";
  r_body += "<a href=\"/set/ac/on\"\"><button>on</button></a>";
  r_body += "<a href=\"/set/ac/off\"><button>off</button></a></p>";
  r_body += "<p>temp: ";
  r_body += String(ac.temp);
  r_body += "<a href=\"/set/ac/temp/21\"\"><button>21</button></a>";
  r_body += "<a href=\"/set/ac/temp/22\"\"><button>22</button></a>";
  r_body += "<a href=\"/set/ac/temp/23\"\"><button>23</button></a>";
  r_body += "<a href=\"/set/ac/temp/24\"\"><button>24</button></a>";
  r_body += "<a href=\"/set/ac/temp/25\"\"><button>25</button></a>";
  r_body += "</p>";
  r_body += "<p>airflow: ";
  r_body += String(ac.flow);
  r_body += "<a href=\"/set/ac/flow/0\"\"><button>min</button></a>";
  r_body += "<a href=\"/set/ac/flow/1\"\"><button>med</button></a>";
  r_body += "<a href=\"/set/ac/flow/2\"\"><button>max</button></a>";
  r_body += "</p>";
  r_body += "<p>swing: ";
  r_body += ac.swing?"On":"Off";
  r_body += "<a href=\"/set/ac/swing/on\"\"><button>On</button></a>";
  r_body += "<a href=\"/set/ac/swing/off\"><button>Off</button></a></p>";

  r_body += "<h2>Environment</h2>";
  r_body += environment.isOn?"On":"Off";
  r_body += "<br/>";
  r_body += "temperature: ";
  r_body += String(environment.temperature);
  r_body += "&#176;<br/>";
  r_body += "humidity: ";
  r_body += String(environment.humidity);
  r_body += "%<br/>";
  r_body += "pressure: ";
  r_body += String(environment.pressure);
  r_body += "hPa<br/>";

  r_body += "<h2>api</h2>";
  r_body += "<a href=\"/status\">status</a></p>";

  r_body += ("</body></html>");
  return r_body;
}

String status_json(){
  String r_body;
  r_body += "{";
  r_body += "\"ac\":" + ac.to_json() + ",";
  r_body += "\"env\":"+ environment.to_json();
  r_body += "}";
  return r_body;
}

String ac_status_json(){
  String r_body;
  r_body += "{\"statusPattern\":\"";
  r_body += ac.isOn?"true":"false";
  r_body += "\"}";
  return r_body;
}

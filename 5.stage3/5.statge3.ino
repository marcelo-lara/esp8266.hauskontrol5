#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup

//shifted out
#include "src/Core/Io/ShiftedIo/ShiftedIo.h"
#define latchPin 13 // D7 violet
#define dataPin  12 // D6 blue
#define clockPin 14 // D5 clock
ShiftedIo shiftedIo(latchPin, dataPin, clockPin, true);

//fan
#include "src/Core/Devices/Fan/Fan.h"
#define defSpeed           3 // default speed
Fan fan(&shiftedIo, defSpeed); // suite fan
///////////////////////////////////////////

#include <ESP8266WebServer.h> // web server
ESP8266WebServer server(80);    

void setup() {
  //connect
  wemosWiFi.connect("stage3");

  // web ui
  server_setup();

}
 
void loop() {
  wemosWiFi.update();
  server.handleClient();

  for (int i = 0; i < 5; i++)
  {
    fan.setSpeed(i);
    delay(1000);
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HTTP_OK  server.send(200, "text/plain", "OK")
#define HTTP_404 server.send(404, "text/plain", "404: Not found")
void server_setup(){

  //handles
  server.on("/",                []() { server.send(200, "text/html", ui_root()); return; });
  server.on("/status",          []() { server.send(200, "application/json", status_json()); return; });

  //undhandleds
  server.onNotFound([](){HTTP_404;});  
  server.begin();

}

String ui_root(){
  String r_body;
  r_body += "<html>";
  r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title></title></head><body>";
  r_body += ("</body></html>");
  return r_body;
}

String status_json(){
  String r_body;
  r_body += "{";
  r_body += "}";
  return r_body;
}

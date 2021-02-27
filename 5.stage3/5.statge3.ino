#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define wifiLedPin      2 // D4 builtIn led
#define statusLedPin    2 // D4 Wall StatusLed

//shifted out
#define latchPin 13 // D7 violet
#define dataPin  12 // D6 blue
#define clockPin 14 // D5 clock
#include "src/Core/Io/ShiftedIo/ShiftedIo.h"
ShiftedIo shiftedIo(latchPin, dataPin, clockPin, true);

//fan
#define defSpeed           3 // default speed
///////////////////////////////////////////

#include <ESP8266WebServer.h> // web server
ESP8266WebServer server(80);    
#define HTTP_OK  server.send(200, "text/plain", "OK")
#define HTTP_404 server.send(404, "text/plain", "404: Not found")


void setup() {
  Serial.begin(250000);
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices

  //connect
  wemosWiFi.connect("stage3");

  // web ui
  server.on("/",                []() { server.send(200, "text/html", ui_root()); return; });
  server.on("/status",          []() { server.send(200, "application/json", status_json()); return; });

  server.onNotFound([](){HTTP_404;});  
  server.begin();

  //non-critical hardware
  analogWrite(statusLedPin, 50);

}
 
void loop() {
  wemosWiFi.update();
  server.handleClient();
}

String ui_root(){
  String r_body;
  r_body += "<html>";
  r_body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>";
  r_body += ("</body></html>");
  return r_body;
}

String status_json(){
  String r_body;
  r_body += "{";
  r_body += "}";
  return r_body;
}

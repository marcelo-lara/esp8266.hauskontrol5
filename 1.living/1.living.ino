#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define wifiLedPin      2 // D4 builtIn led
#define wallSwitchPin   5 // D1 Wall Switch
#define statusLedPin    2 // D4 Wall StatusLed
#define out1            4 // D2
#define out2           14 // D5
#define out3           12 // D6
#define out4           13 // D7

WiFiServer server(80);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light[] = {
  Light(out1, false), // main
  Light(out2, false), // dicro
  Light(out3, false), // bookshelf
  Light(out4, false) // corner
};

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  light[0].turnOn();

  //connect
  wemosWiFi.connect("living");
  light[0].turnOff();

  // Start the server
  server.begin();
 
  //non-critical hardware
  analogWrite(statusLedPin, 50);

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  wemosWiFi.update();

  wallSwitch.update();
  handleConnection();

 
}

void handleConnection(){
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) return;
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){delay(1);}
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  if (request.indexOf("/status/") != -1)
    return replyStatus(&client, &request);

  if (request.indexOf("/set/1/on") != -1)     light[0].turnOn();
  if (request.indexOf("/set/1/off") != -1)    light[0].turnOff();
  if (request.indexOf("/set/1/toggle") != -1) light[0].toggle();
 
  if (request.indexOf("/set/2/on") != -1)     light[1].turnOn();
  if (request.indexOf("/set/2/off") != -1)    light[1].turnOff();
  if (request.indexOf("/set/2/toggle") != -1) light[1].toggle();
 
  if (request.indexOf("/set/3/on") != -1)     light[2].turnOn();
  if (request.indexOf("/set/3/off") != -1)    light[2].turnOff();
  if (request.indexOf("/set/3/toggle") != -1) light[2].toggle();
 
  if (request.indexOf("/set/4/on") != -1)     light[3].turnOn();
  if (request.indexOf("/set/4/off") != -1)    light[3].turnOff();
  if (request.indexOf("/set/4/toggle") != -1) light[3].toggle();
 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body>");
  for (size_t i = 0; i < 4; i++)
    client.printf("io %i: %s <a href=\"/set/%i/toggle\"\"><button>switch</button></a><br>", i, (light[i].isOn?"On":"Off"), i );
  client.println("</body></html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

};

void replyStatus(WiFiClient *cl, String *req){
  
  int st_from = req->indexOf("/status/") + 8;
  int pos = req->substring(st_from, st_from+1).toInt()-1;

  cl->println("HTTP/1.1 200 OK");
  cl->println("Content-Type: application/json");
  cl->println(""); 
  cl->print("{\"statusPattern\": \""); 
  cl->print(light[pos].isOn?"true":"false"); 
  cl->println("\"}");
}

void switchCallback(int clicks) {
  Serial.print(clicks);
  Serial.println("");
  
  switch (clicks)
  {
  case -1:
    turnAllOff();
    break;
  case 1:
    if(light[0].isOn){
      turnAllOff();
    }else{
      light[0].turnOn();
    };
    break;

  case 2:
    light[1].toggle();
    break;

  case 3:
    light[2].toggle();
    break;

  case 4:
    light[3].toggle();
    break;

  default:
    break;
  }
}

void turnAllOff(){
  light[0].turnOff();
  light[1].turnOff();
  light[2].turnOff();
  light[3].turnOff();
}
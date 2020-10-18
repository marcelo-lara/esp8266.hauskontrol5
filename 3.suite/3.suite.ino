#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define wifiLedPin      2 // D4 builtIn led
#define wallSwitchPin   5 // D1 Wall Switch
#define statusLedPin    2 // D4 Wall StatusLed

//light
#define lightOut        4 // D2

//fan
#define fanOut1           13 // D7
#define fanOut2           12 // D6
#define fanOut3           14 // D5
#define fanOut4           15 // D8
#define defSpeed           4 // default speed
///////////////////////////////////////////

WiFiServer server(80);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(lightOut, true); // main
void handleLight(String *args);

#include "src/Core/Devices/Fan/Fan.h"
Fan fan(fanOut1, fanOut2, fanOut3, fanOut4, defSpeed); // suite fan

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  light.turnOn();

  //connect
  wemosWiFi.connect("suite");
  light.turnOff();

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
  while(!client.available()){delay(1);}
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.print("request> ");
  Serial.println(request);
  client.flush();

  // Match the request
  if (request.indexOf("/status/light/1") != -1)  {
    client.println("HTTP/1.1 200 OK");client.println("Content-Type: application/json");client.println(""); client.print("{\"statusPattern\": \""); client.print(light.isOn?"true":"false"); client.println("\"}");
    return;
  }

  //light
  if (request.indexOf("/set/light/") != -1) handleLight(&request);
 
  //fan
  if (request.indexOf("/set/fan/") != -1) handleFan(&request);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.printf("<h1>%s</h1>","suite" );
 
  client.printf("light: %s <a href=\"/set/light/1/toggle\"\"><button>switch</button></a><br>", (light.isOn?"On":"Off") );
  client.printf("fan:   %i \
    <a href=\"/set/fan/off\"\"><button>off</button></a>\
    <a href=\"/set/fan/speed/1\"\"><button>1</button></a>\
    <a href=\"/set/fan/speed/2\"\"><button>2</button></a>\
    <a href=\"/set/fan/speed/3\"\"><button>3</button></a>\
    <a href=\"/set/fan/speed/4\"\"><button>4</button></a>\
    <a href=\"/set/fan/on\"\"><button>on</button></a>\
    <br>", (fan.speed) );

  client.println("</html>");
 
  delay(1);
};

void handleLight(String *args){
  if (args->indexOf("/set/light/1/on") != -1)     light.turnOn();
  if (args->indexOf("/set/light/1/off") != -1)    light.turnOff();
  if (args->indexOf("/set/light/1/toggle") != -1) light.toggle();
};

void handleFan(String *args){
  if (args->indexOf("/set/fan/on") != -1)      fan.turnOn();
  if (args->indexOf("/set/fan/off") != -1)     fan.turnOff();
  if (args->indexOf("/set/fan/toggle") != -1)  fan.toggle();
  if (args->indexOf("/set/fan/speed/1") != -1) fan.setSpeed(1);
  if (args->indexOf("/set/fan/speed/2") != -1) fan.setSpeed(2);
  if (args->indexOf("/set/fan/speed/3") != -1) fan.setSpeed(3);
  if (args->indexOf("/set/fan/speed/4") != -1) fan.setSpeed(4);
};


void switchCallback(int clicks) {
  Serial.print(clicks);
  Serial.println("");
  light.toggle();  
}

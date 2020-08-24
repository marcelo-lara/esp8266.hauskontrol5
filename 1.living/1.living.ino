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
Light light1(out1, false); // main
Light light2(out2, false); // dicro
Light light3(out3, false); // bookshelf
Light light4(out4, false); // corner

void setup() {
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);

  //core devices
  wallSwitch.setup();
  wallSwitch.swCallback=switchCallback;
  light1.turnOn();

  //connect
  wemosWiFi.connect("living");
  light1.turnOff();

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
  if (request.indexOf("/status/1") != -1)  {
    client.println("HTTP/1.1 200 OK");client.println("Content-Type: application/json");client.println(""); client.print("{\"statusPattern\": \""); client.print(light1.isOn?"true":"false"); client.println("\"}");
    return;
  }

  if (request.indexOf("/status/2") != -1)  {
    client.println("HTTP/1.1 200 OK");client.println("Content-Type: application/json");client.println(""); client.print("{\"statusPattern\": \""); client.print(light2.isOn?"true":"false"); client.println("\"}");
    return;
  }

  if (request.indexOf("/status/3") != -1)  {
    client.println("HTTP/1.1 200 OK");client.println("Content-Type: application/json");client.println(""); client.print("{\"statusPattern\": \""); client.print(light3.isOn?"true":"false"); client.println("\"}");
    return;
  }

  if (request.indexOf("/status/4") != -1)  {
    client.println("HTTP/1.1 200 OK");client.println("Content-Type: application/json");client.println(""); client.print("{\"statusPattern\": \""); client.print(light4.isOn?"true":"false"); client.println("\"}");
    return;
  }

  if (request.indexOf("/set/1/on") != -1)     light1.turnOn();
  if (request.indexOf("/set/1/off") != -1)    light1.turnOff();
  if (request.indexOf("/set/1/toggle") != -1) light1.toggle();
 
  if (request.indexOf("/set/2/on") != -1)     light2.turnOn();
  if (request.indexOf("/set/2/off") != -1)    light2.turnOff();
  if (request.indexOf("/set/2/toggle") != -1) light2.toggle();
 
  if (request.indexOf("/set/3/on") != -1)     light3.turnOn();
  if (request.indexOf("/set/3/off") != -1)    light3.turnOff();
  if (request.indexOf("/set/3/toggle") != -1) light3.toggle();
 
  if (request.indexOf("/set/4/on") != -1)     light4.turnOn();
  if (request.indexOf("/set/4/off") != -1)    light4.turnOff();
  if (request.indexOf("/set/4/toggle") != -1) light4.toggle();
 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.printf("io 1: %s <a href=\"/set/1/toggle\"\"><button>switch</button></a><br>", (light1.isOn?"On":"Off") );
  client.printf("io 2: %s <a href=\"/set/2/toggle\"\"><button>switch</button></a><br>", (light2.isOn?"On":"Off") );
  client.printf("io 3: %s <a href=\"/set/3/toggle\"\"><button>switch</button></a><br>", (light3.isOn?"On":"Off") );
  client.printf("io 4: %s <a href=\"/set/4/toggle\"\"><button>switch</button></a><br>", (light4.isOn?"On":"Off") );

  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

};



void switchCallback(int clicks) {
  Serial.print(clicks);
  Serial.println("");
  
  switch (clicks)
  {
  case -1:
    turnAllOff();
    break;
  case 1:
    if(light1.isOn){
      turnAllOff();
    }else{
      light1.turnOn();
    };
    break;

  case 2:
    light2.toggle();
    break;

  case 3:
    light3.toggle();
    break;

  case 4:
    light4.toggle();
    break;

  default:
    break;
  }
}

void turnAllOff(){
  light1.turnOff();
  light2.turnOff();
  light3.turnOff();
  light4.turnOff();
}
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
#define out1           15 // D8
#define out2           14 // D5
#define out3           12 // D6
#define out4           13 // D7

WiFiServer server(80);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(lightOut, true); // main

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
  Serial.println("new client");
  while(!client.available()){delay(1);}
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  if (request.indexOf("/status/1") != -1)  {
    client.println("HTTP/1.1 200 OK");client.println("Content-Type: application/json");client.println(""); client.print("{\"statusPattern\": \""); client.print(light.isOn?"true":"false"); client.println("\"}");
    return;
  }

  if (request.indexOf("/set/1/on") != -1)     light.turnOn();
  if (request.indexOf("/set/1/off") != -1)    light.turnOff();
  if (request.indexOf("/set/1/toggle") != -1) light.toggle();
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.printf("<h1>%s</h1>","suite" );
 
  client.printf("light 1: %s <a href=\"/set/1/toggle\"\"><button>switch</button></a><br>", (light.isOn?"On":"Off") );

  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

};



void switchCallback(int clicks) {
  Serial.print(clicks);
  Serial.println("");
  light.toggle();  
}

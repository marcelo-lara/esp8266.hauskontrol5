#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "src/wemos.setup/wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define irSendPin      14 // D5 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led

WiFiServer server(80);

#include "src/Core/Devices/AC/AC.h"
AC ac(irSendPin, 24);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;


void setup() {

  //core devices
  ac.init();

  //connect
  wemosWiFi.connect("officeac");

  // Start the server
  server.begin();
 
  //non-critical hardware
  environment.setup();

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  wemosWiFi.update();

  environment.update();
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
 
  if (request.indexOf("/status/") != -1) 
    return handleStatus(&request, &client);

  if (request.indexOf("/set/") != -1) 
    handleCommand(&request, &client);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
//ac
  client.print("<h2>AC</h2> ");
  client.print(ac.isOn?"On":"Off");
  client.println("<a href=\"/set/ac/on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/set/ac/off\"> <button>Turn Off </button></a><br />");  

//environment
  client.print("<h2>Environment</h2>");
  client.print(environment.isOn?"On":"Off");
  client.println("<br/>");
  client.print("temperature: ");
  client.print(environment.temperature);
  client.println("°<br/>");
  client.print("humidity: ");
  client.print(environment.humidity);
  client.println("%<br/>");
  client.print("pressure: ");
  client.print(environment.pressure);
  client.println("hPa<br/>");

  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

};

void handleStatus(String *args, WiFiClient *client){
  client->println("HTTP/1.1 200 OK");
  client->println("Content-Type: application/json");
  client->println(""); //  do not forget this one
  bool _status = false;

  //ac
  if (args->indexOf("/status/ac") != -1)  
    _status = ac.isOn;

  //env sensor
  if (args->indexOf("/status/env") != -1)  
    _status = environment.isOn;

  client->print("{\"statusPattern\": \""); 
  client->print(_status?"true":"false"); 
  client->println("\"}");

}

void handleCommand(String *args, WiFiClient *client){

  //air conditioner
  if (args->indexOf("/set/ac/") != -1){
    // on/off
    if (args->indexOf("/set/ac/on") != -1) ac.turnOn();
    if (args->indexOf("/set/ac/off") != -1) ac.turnOff();

  } 

}

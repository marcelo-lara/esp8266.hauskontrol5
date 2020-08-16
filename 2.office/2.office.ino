#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wemos.setup.h"

/////////////////////////////////////////
// Hardware Setup
#define wallSwitchPin  14 // D5 Wall Switch
#define statusLedPin   12 // D6 Wall StatusLed
#define relayOutPin    13 // D7 relay out
#define irSendPin       0 // D3 infrared signal out
#define bme280_scl      5 // D1 SCL (bme280)
#define bme280_sda      4 // D2 SDA (bme280)
#define wifiLedPin      2 // D4 builtIn led

WiFiServer server(80);

#include "src/Core/Devices/Button/Button.h"
Button wallSwitch(wallSwitchPin);

#include "src/Core/Devices/Light/Light.h"
Light light(relayOutPin, true);

#include "src/Core/Devices/Environment/Environment.h"
Environment environment;


void setup() {
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
  server.begin();
 
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
  wemosWiFi.update();

  wallSwitch.update();
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
 
  // Match the request
  if (request.indexOf("/STATUS") != -1)  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println(""); //  do not forget this one
    client.print("{\"statusPattern\": \"");
    client.print(light.isOn?"true":"false");
    client.println("\"}");
    return;
  }

  if (request.indexOf("/LED=ON") != -1)  light.turnOn();
  if (request.indexOf("/LED=OFF") != -1)  light.turnOff();
 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("ligth: ");
  client.print(light.isOn?"On":"Off");
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  

//environment
  client.println("<br/><br/>");
  client.print("temperature:");
  client.print(environment.temperature);
  client.println("<br/>");
  client.print("humidity:");
  client.print(environment.humidity);
  client.println("<br/>");
  client.print("pressure:");
  client.print(environment.pressure);
  client.println("<br/>");
  
  

  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

};
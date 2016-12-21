/**
  * Sensor-NET
  *
  * Standalone Web-Based Temperature and Humidity Sensor
  *
  * 
  */

#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#define DHTPIN 2
#define DHTTYPE DHT22

float temperature;
float humidity;
float heatIndex;

ESP8266WebServer server(80);
WiFiManager wifiManager;

DHT dht(DHTPIN, DHTTYPE);


void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleReset() {
  WiFi.disconnect();
  delay(3000);
  ESP.reset();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void readTemperature() {

  float h = dht.readHumidity();

  if (!isnan(h)) {
    humidity = h;
  }

  float t = dht.readTemperature();

  if (!isnan(t)) {
    temperature = t;
  }

}

void setup() {

  Serial.begin(115200);

  delay(100);
  Serial.println("Starting");

  wifiManager.autoConnect("SENSOR-NET", "12345678");

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("temperature.local")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web Server started");
  dht.begin();

}

void loop() {
  server.handleClient();

  delay(2000);

  readTemperature();
}

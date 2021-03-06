/**
  * Sensor-NET
  *
  * Standalone Web-Based Temperature and Humidity Sensor
  *
  * 
  */
#include <TaskScheduler.h>
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

#define TASK_WEBSERVER  100
#define TASK_SENSOR     2000

Scheduler runner;

ESP8266WebServer server(80);
WiFiManager wifiManager;

DHT dht(DHTPIN, DHTTYPE);

void webserverCallback();
void sensorCallback();

Task tWebserver(TASK_WEBSERVER, TASK_FOREVER, &webserverCallback);
Task tSensor(TASK_SENSOR, TASK_FOREVER, &sensorCallback);

void handleRoot() {
  String temperatureStr = convertFloat(temperature);
  String humidityStr = convertFloat(humidity);
  String content = "";

  content+=("<html><head>");
  content+=("<title>Dominik's Temperatur Sensor</title>");
  content+=("<meta http-equiv=\"refresh\" content=\"5\" />");
  content+=("</head>");
  content+=("<body>");
  content+=("<div>");
  content+=("<h1>Temperatur Sensor</h1>");
  content+=("<p>Frohe Weihnachten, Bro.</p>");
  content+=("<div>Temperatur: " + temperatureStr + "</div>");
  content+=("<div>Luftfeuchtigkeit: " + humidityStr + "</div>");
  content+=("</div>");
  content+=("</body>");
  content+=("</html>");

  server.send(200, "text/html", content);
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

String convertFloat(float input) {
  String stringVal = "";
  char charVal[10];

  dtostrf(input, 4, 4, charVal);
  for(int i=0;i<sizeof(charVal);i++)
  {
    stringVal+=charVal[i];
  }

  return stringVal;

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
  wifi_station_set_hostname("temperature");

  Serial.begin(74880);

  delay(100);
  Serial.println("Starting");

  wifiManager.autoConnect("SENSOR-NET", "12345678");
 
  if (!MDNS.begin("temperature.local")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS responder started");

  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web Server started");
  dht.begin();

  runner.init();
  runner.addTask(tWebserver);
  runner.addTask(tSensor);
  delay(2000);
  tWebserver.enable();
  tSensor.enable();

}

void webserverCallback() {
    server.handleClient();
}

void sensorCallback() {
    readTemperature();
}

void loop() {
  runner.execute();
}

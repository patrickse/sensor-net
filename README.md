# Sensor-Net

Little Arduino Sketch as a secret-santa gift for my brother.

It actually reads the temperature from a DHT-22 and presents it as a webpage. I used the sketch on an ESP-8266.

I actually plugged together some really cool arduino libraries.. so many thanks to 
 - https://github.com/tzapu/WiFiManager
 - and the ESP8266 folks... making it easy for an arduino guy to program it...

## Getting Started

After loading the sketch onto your ESP-8266 the system will come up with a little adhoc wifi network. Connect to this network with your fone/tablet/pc and a captive portal should appear.

Configure your private WiFi and your done. You should now be able to browse the device by opening http://temperature.local.

The Password for the adhoc wifi is `12345678`. Feel free to modify ;)...

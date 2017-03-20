# AzureIoTHubMQTTClientSample
AzureIoTHubMQTTClient example for ESPectro board, using Platformio.

## AzureIoTHubMQTTClient
[AzureIoTHubMQTTClient](https://github.com/andriyadi/AzureIoTHubMQTTClient) is a ESP8266 Arduino library I made to work with Azure IoT Hub via MQTT protocol. 

## How to Use
### PlatformIO
Just do `pio run -t upload`
### Arduin IDE
Rename the source code file from `AzureIoTHubSample.cpp` to `AzureIoTHubSample.ino`, then open the `.ino` file.

## Dependencies
This project sample is making use of `AzureIoTHubMQTTClient` library I made that have dependencies to following libraries:
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [Time](https://github.com/PaulStoffregen/Time)

This sample also uses [Adafruit-BMP085-Library](https://github.com/adafruit/Adafruit-BMP085-Library).

Please download all required libraries, either using `pio lib` for PlatformIO, or Library Manager for Arduino IDE>

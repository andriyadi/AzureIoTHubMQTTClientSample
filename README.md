# AzureIoTHubMQTTClientSample
AzureIoTHubMQTTClient example for ESPectro board, using Platformio or Arduino IDE.

## Demo
[![Demo](https://img.youtube.com/vi/oPRf79p3-Wc/0.jpg)](https://www.youtube.com/watch?v=oPRf79p3-Wc)

## AzureIoTHubMQTTClient
[AzureIoTHubMQTTClient](https://github.com/andriyadi/AzureIoTHubMQTTClient) is a ESP8266 Arduino library I made to work with Azure IoT Hub via MQTT protocol. 

## How to Use
### PlatformIO
Just do `pio run -t upload`
### Arduino IDE
Rename the source code file from `AzureIoTHubSample.cpp` to `AzureIoTHubSample.ino`, then open the `.ino` file with Arduino IDE.

## Dependencies
This project sample is making use of `AzureIoTHubMQTTClient` library I made that have dependencies to following libraries:
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [Time](https://github.com/PaulStoffregen/Time)

This sample also uses [Adafruit-BMP085-Library](https://github.com/adafruit/Adafruit-BMP085-Library) if you have the sensor.

Please download all required libraries, either using `pio lib` for PlatformIO, or Library Manager for Arduino IDE.

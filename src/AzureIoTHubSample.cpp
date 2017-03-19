//
// Created by Andri Yadi on 3/19/17.
//

#include <Arduino.h>
#include "ESPectro.h"
#include "DCX_AppSetting.h"
#include "DCX_WifiManager.h"
#include "AzureIoTHubMQTTClient.h"
#include "Time.h"
#include <ESP8266mDNS.h>

ESPectro board;
DCX_WifiManager wifiManager(AppSetting);

// Azure IoT Hub Settings --> CHANGE THESE
#define IOTHUB_HOSTNAME         "dycodex.azure-devices.net"
#define DEVICE_ID               "espectro-01"
#define DEVICE_KEY              "nhWXiles4WXOsM7DQjnTCrTk8zuZGdK55hHYF3BniLU=" //Primary key of the device

#define USE_BMP180              1 //Set this to 0 if you don't have the sensor and generate random sensor value to publish

#if USE_BMP180
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
#endif

WiFiClientSecure tlsClient;
AzureIoTHubMQTTClient client(tlsClient, IOTHUB_HOSTNAME, DEVICE_ID, DEVICE_KEY);
unsigned long lastPublishMillis = 0;

void connectToIoTHub() {

    Serial.print("\nBeginning Azure IoT Hub Client... ");
    if (client.begin()) {
        Serial.println("OK");
    } else {
        Serial.println("Could not connect to MQTT");
    }
}

void onClientEvent(const AzureIoTHubMQTTClient::AzureIoTHubMQTTClientEvent event) {
    if (event == AzureIoTHubMQTTClient::AzureIoTHubMQTTClientEventConnected) {
        DEBUG_SERIAL("Connected to Azure IoT Hub\n");

        //turn off led animation upon connected
        board.stopLEDAnimation();
    }
}

void onActivateRelayCommand(String cmdName, JsonVariant jsonValue) {

    //Parse cloud-to-device message JSON. In this example, I send the command message with following format:
    //{"Name":"ActivateRelay","Parameters":{"Activated":0}}

    JsonObject& jsonObject = jsonValue.as<JsonObject>();
    if (jsonObject.containsKey("Parameters")) {
        auto params = jsonValue["Parameters"];
        auto isAct = (params["Activated"]);
        if (isAct) {
            Serial.println("Activated true");
            board.turnOnLED();
        }
        else {
            Serial.println("Activated false");
            board.turnOffLED();
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    while (!Serial); // wait for serial attach
    delay(2000);

    AppSetting.load();
    board.turnOffAllNeopixel();

#if USE_BMP180
    if (bmp.begin()) {
        Serial.println("BMP INIT SUCCESS");
    }
#endif

    wifiManager.onWifiConnectStarted([]() {
        DEBUG_SERIAL("WIFI CONNECTING STARTED\r\n");
        board.fadeLED(1300);
    });

    wifiManager.onWifiConnected([board](boolean newConn) {
        DEBUG_SERIAL("WIFI CONNECTED. IP Address: %s\r\n", WiFi.localIP().toString().c_str());

        //Connect to IoT Hub when WiFi connected
        connectToIoTHub();
    });

    wifiManager.onWifiConnecting([](unsigned long elapsed) {
        //DEBUG_SERIAL("%d\r\n", elapsed);
        //board.toggleLED();
    });

    wifiManager.onWifiDisconnected([](WiFiDisconnectReason reason) {
        DEBUG_SERIAL("WIFI GIVE UP\r\n");
        board.stopLEDAnimation();
    });

//    wifiManager.begin();
//    wifiManager.begin("Andromax-M3Y-C634", "p@ssw0rd");
    wifiManager.begin("DyWare-AP3", "p@ssw0rd");


    //Handle Azure IoT Hub client events
    client.onEvent(onClientEvent);

    //Add command to handle and its handler
    //Command format is assumed like this: {"Name":"[COMMAND_NAME]","Parameters":[PARAMETERS_JSON]}
    client.onCloudCommand("ActivateRelay", onActivateRelayCommand);
}

void readSensor(float *temp, float *press) {

#if USE_BMP180
    *temp = bmp.readTemperature();
    *press = 1.0f*bmp.readPressure()/1000; //--> kilo
#else
    //If you don't have the sensor
    *temp = 20 + (rand() % 10 + 2);
    *press = 90 + (rand() % 8 + 2);
#endif

}

void loop() {
    wifiManager.run();
    client.run();

    if (client.connected()) {
        // Publish a message roughly every 3 second. Only after time is retrieved and set properly.
        if(millis() - lastPublishMillis > 3000 && timeStatus() != timeNotSet) {
            lastPublishMillis = millis();

            //Read the actual temperature from sensor
            float temp, press;
            readSensor(&temp, &press);

            //Get current timestamp, using Time lib
            time_t currentTime = now();

            // You can do this to publish payload to IoT Hub
            /*
            String payload = "{\"DeviceId\":\"" + String(DEVICE_ID) + "\", \"MTemperature\":" + String(temp) + ", \"EventTime\":" + String(currentTime) + "}";
            Serial.println(payload);

            //client.publish(MQTT::Publish("devices/" + String(DEVICE_ID) + "/messages/events/", payload).set_qos(1));
            client.sendEvent(payload);
            */

            //Or instead, use this more convenient way
            AzureIoTHubMQTTClient::KeyValueMap keyVal = {{"MTemperature", temp}, {"MPressure", press}, {"DeviceId", DEVICE_ID}, {"EventTime", currentTime}};
            client.sendEventWithKeyVal(keyVal);
        }
    }
}
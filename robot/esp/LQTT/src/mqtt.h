#ifndef MQTT_H
#define MQTT_H

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// https://byfeel.info/client-mqtt-dans-votre-module-esp/

namespace mqtt {

    //WiFi Connection configuration
    char ssid[] = "HUAWEI P Qandre";     //  le nom du reseau WIFI
    char password[] = "qandre211198";  // le mot de passe WIFI
    //mqtt server
    char mqtt_server[] = "192.168.43.29";  //adresse IP serveur


    WiFiClient espClient;
    PubSubClient MQTTclient(espClient);

    void init() {

        WiFi.begin(ssid, password);
        while(WiFi.status() != WL_CONNECTED) {
            delay(500);
        }

        MQTTclient.setServer(mqtt_server, 20000);

        while(!MQTTclient.connected()) {

            String clientId = "TestClient-" + String(random(0xffff), HEX);
            if(!MQTTclient.connect(clientId.c_str())) {
                delay(500);
            }
        }
    }

    void test() {

        String response = "test";
        MQTTclient.publish("qtmqtt/robot", response.c_str());

    }


}

#endif
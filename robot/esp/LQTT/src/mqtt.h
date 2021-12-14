#ifndef MQTT_H
#define MQTT_H

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <PubSubClient.h>

// https://byfeel.info/client-mqtt-dans-votre-module-esp/

namespace mqtt {

    // WiFi Connection configuration
    char wifi_ssid[] = "IMERIR_IoT";     //  le nom du reseau WIFI : IMERIR_IoT, HUAWEI P Qandre
    char wifi_password[] = "kohWoong5oox";  // le mot de passe WIFI : kohWoong5oox, qandre211198
    
    // MQTT server properties
    char mqtt_server[] = "mqtt-milles.imerir.org"; //adresse IP serveur
    uint16_t mqtt_port = 1884;
    String mqtt_id = "robot5";
    String mqtt_user = "terrain2";
    String mqtt_password = "56jpwYhr";

    // Clients
    WiFiClient espClient;
    PubSubClient MQTTclient(espClient);

    void init();
    void callback(char* topic, byte* payload, unsigned int length);
    void mqtt::send(String message);
    void loop();

}

#endif
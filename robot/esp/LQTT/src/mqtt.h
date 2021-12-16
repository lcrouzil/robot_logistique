#ifndef MQTT_H
#define MQTT_H

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <PubSubClient.h>

// https://byfeel.info/client-mqtt-dans-votre-module-esp/

namespace mqtt {

    // Robot
    //String robot_id = "ROBOT5";

    // WiFi Connection configuration
    char wifi_ssid[] = "Livebox-A42C";     //  le nom du reseau WIFI : IMERIR_IoT, HUAWEI P Qandre
    char wifi_password[] = "FEEE619AFE5CCEE219DE7AFDED";  // le mot de passe WIFI : kohWoong5oox, qandre211198
    
    // MQTT server properties
    char mqtt_server[] = "mqtt-milles.imerir.org"; //adresse IP serveur
    uint16_t mqtt_port = 1884;
    String mqtt_id = "robot5-zrcgdrthvygun";
    String mqtt_user = "terrain2";
    String mqtt_password = "56jpwYhr";

    // Clients
    WiFiClient espClient;
    PubSubClient MQTTclient(espClient);

    void init();
    void callback(char* topic, byte* payload, unsigned int length);
    void send(String message);
    void loop();

    void connect() {
        MQTTclient.setServer(mqtt_server, mqtt_port);
        MQTTclient.setCallback(mqtt::callback);
        //MQTTclient.setSocketTimeout(10000);
        while(!MQTTclient.connected()) {

            if(!MQTTclient.connect(mqtt_id.c_str(), mqtt_user.c_str(), mqtt_password.c_str())) {
                
                digitalWrite(D0, LOW);
                delay(400);
                digitalWrite(D0, HIGH);
                delay(100);
            }

            //Serial.println(MQTTclient.state());

        }

        digitalWrite(D0, HIGH);
        
        //Serial.println("MQTT set");

        MQTTclient.subscribe("field/robot/ROBOT5/path");
    }

    void init() {

        //Serial.println("Post WiFi");

        // Connecte le WiFi
        WiFi.mode(WIFI_OFF);    //Empêcher les problèdes de reconnexion
        delay(1000);
        WiFi.mode(WIFI_STA);    //Cette ligne masque la visualisation de l'ESP en tant que hotspot wifi

        WiFi.begin(wifi_ssid, wifi_password);
        while(WiFi.status() != WL_CONNECTED) {

            digitalWrite(D0, LOW);
            delay(100);
            digitalWrite(D0, HIGH);
            delay(400);
        }

        //Serial.print("WiFi set : ");
        //IPAddress addr = WiFi.localIP();
        //Serial.println(addr.toString());
        //Serial.println(WiFi.subnetMask());
        //Serial.print("MAC Address : ");
        //Serial.println(WiFi.macAddress());

        // Connecte le serveur()
        //Serial.print(mqtt_server); Serial.print(":");Serial.println(mqtt_port);
        
        mqtt::connect();

    }

    /**
     * @brief Récupère les messages reçues depuis le broker, et les renvoies sur la liaison série
     * 
     * @param topic Topic du message
     * @param payload Payload du message
     * @param length Taille du message
     */
    void callback(char* topic, byte* payload, unsigned int length) {
        Serial.print(topic);
        Serial.print(" ");

        digitalWrite(D0, HIGH);
        delay(100);
        digitalWrite(D0, LOW);
        delay(100);
        digitalWrite(D0, HIGH);

        payload[length] = '\0';
        String s = String((char*)payload);

        Serial.print(s + "\n\n");
    }

    void send(String message) {

        //Serial.println(MQTTclient.state());

        while(!MQTTclient.connected()) {
                digitalWrite(D0, LOW);
                delay(1000);
                digitalWrite(D0, HIGH);
                mqtt::connect();
                digitalWrite(D0, LOW);
        }

        //Serial.println("mqtt::send");

        uint8_t i = message.indexOf(" ");
        uint8_t j = message.length();
        const char* c_message = message.c_str();
        char topic[i +1] = {0};
        char content[j -i +1] = {0};
        strncpy(topic, c_message, i);
        strncpy(content, c_message +i +1, j -i -3);

        // Serial.println(message);
        //Serial.println("." + String(topic) + ".");
        //Serial.println("." + String(content) + ".");
        if(MQTTclient.publish(topic, content)) {

            digitalWrite(D0, HIGH);
            delay(100);
            digitalWrite(D0, LOW);
            delay(100);
            digitalWrite(D0, HIGH);
            delay(100);
            digitalWrite(D0, LOW);
            delay(100);
            digitalWrite(D0, HIGH);
        }
        else {
            digitalWrite(D0, HIGH);
            delay(1000);
            digitalWrite(D0, LOW);
            mqtt::connect();
            digitalWrite(D0, HIGH);
        }
    }

    void loop() {

        MQTTclient.loop();

    }
}

#endif
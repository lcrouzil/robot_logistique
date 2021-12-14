#include "mqtt.h"

void mqtt::init() {

    // Serial.println("Post WiFi");

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

    Serial.print("WiFi set : ");
    IPAddress addr = WiFi.localIP();
    Serial.println(addr.toString());
    Serial.println(WiFi.subnetMask());
    Serial.print("MAC Address : ");
    Serial.println(WiFi.macAddress());

    // Connecte le serveur()
    Serial.print(mqtt_server); Serial.print(":");Serial.println(mqtt_port);
    MQTTclient.setKeepAlive(true);
    MQTTclient.setServer(mqtt_server, mqtt_port);
    MQTTclient.setCallback(mqtt::callback);
    while(!MQTTclient.connected()) {

        Serial.println(MQTTclient.state());

        if(!MQTTclient.connect(mqtt_id.c_str(), mqtt_user.c_str(), mqtt_password.c_str())) {
            
            digitalWrite(D0, LOW);
            delay(400);
            digitalWrite(D0, HIGH);
            delay(100);
        }

    }

    digitalWrite(D0, HIGH);
    
    // Serial.println("MQTT set");

    MQTTclient.subscribe("#");

}

/**
 * @brief Récupère les messages reçues depuis le broker, et les renvoies sur la liaison série
 * 
 * @param topic Topic du message
 * @param payload Payload du message
 * @param length Taille du message
 */
void mqtt::callback(char* topic, byte* payload, unsigned int length) {
    Serial.print(topic);
    Serial.print(" ");

    payload[length] = '\0';
    String s = String((char*)payload);

    Serial.println(s);
    Serial.println();
}

void mqtt::send(String message) {

    if(MQTTclient.connected()) {

        Serial.println("mqtt::send");

        uint8_t i = message.indexOf(" ");
        uint8_t j = message.length();
        const char* c_message = message.c_str();
        char topic[i +1] = {0};
        char content[j -i +1] = {0};
        strncpy(topic, c_message, i);
        strncpy(content, c_message +i +1, j -i -3);

        // Serial.println(message);
        Serial.println("." + String(topic) + ".");
        Serial.println("." + String(content) + ".");
        Serial.println(MQTTclient.publish(topic, content));
    }
}

void mqtt::loop() {

    Serial.println("mqtt::loop");

    MQTTclient.loop();
}
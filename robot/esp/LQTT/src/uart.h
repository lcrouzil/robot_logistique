#ifndef UART_H
#define UART_H

#include <Arduino.h>

namespace uart {

    const char separator = '-';
    const uint16_t buffer_size = 1000;
    char buffer[buffer_size] = {0};
    uint16_t buffer_i = 0;

    /**
     * @brief Initialise la partie communicant via l'UART
     * 
     */
    void init() {

        Serial.begin(115200);

        // Serial.println("Serial begin");
    }


    /**
     * @brief Lit les octets entrant de l'UART, retourne une chaine de caractères contenant un message s'il y en a un
     * 
     * @return String Chaine de caractère contenant le topic et le message
     */
    String read() {

        // Serial.println("\nuart::read");
        // Serial.println("." + String(buffer) + ".");
        
        // Lit les données en entré de la liaison série
        uint16_t i = Serial.readBytes(buffer +buffer_i, buffer_size -buffer_i);

        //Serial.print("in : "); Serial.println(i);

        buffer_i += i;
        buffer[buffer_i] = 0;

        // Serial.print("buff : "); Serial.println(buffer_i);

        // Recherche un message complet
        i = 0;
        uint8_t c = '\0';
        uint8_t c_prev = '\0';
        while(  (c != separator || c != c_prev) && i < buffer_i  ) {
            c_prev = c;
            c = buffer[i];
            i++;
        }

        // Serial.print("at : "); Serial.println(i);

        // Extrait le message s'il est complet
        if(c == c_prev) {

            char c_msg[1000] = {0};
            strncpy(c_msg, buffer, i);
            String msg(c_msg);

            uint16_t j = 0, k = i;
            while(i < buffer_size) {
                buffer[j] = buffer[i];
                j++; i++;
            }
            buffer_i -= k;

            return msg;
        }

        return "";
    }

    /**
     * @brief Envoie toutes les données, reçues sur Rx, sur Tx
     * 
     */
    void echo() {
        Serial.print(Serial.readString());
    }

}

#endif
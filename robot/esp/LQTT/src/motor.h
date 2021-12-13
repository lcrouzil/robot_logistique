#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

namespace motor {

    const int VITESSE = 750;  

    void init() {

        pinMode(D1, OUTPUT);  // Moteur Gauche  
        pinMode(D2, OUTPUT);  // Moteur Droit
        pinMode(D3, OUTPUT);  // Sens rotation roue gauche
        pinMode(D4, OUTPUT);  // Sens rotation roue droite
    }

    void set() {

        digitalWrite(D3, HIGH);
        digitalWrite(D4, HIGH);

        analogWrite(D1, VITESSE);
        analogWrite(D2, VITESSE);

    }

    void reset() {

        digitalWrite(D3, HIGH);
        digitalWrite(D4, HIGH);

        analogWrite(D1, 0);
        analogWrite(D2, 0);

    }

}

#endif
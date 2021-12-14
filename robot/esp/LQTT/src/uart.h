#ifndef UART_H
#define UART_H

#include <Arduino.h>

namespace uart {

    enum Command_e {
        STOP = 0x10,
        PAUSE = 0x11,
        RESUME = 0x12,
        RIGHT = 0x13,
        LEFT = 0x14,
        EMPTY = 0x20,
        CHARGED = 0x21
    };
    typedef enum Command_e Command_t;

    void init() {
        Serial.begin(115200);
    }

    int read() {
        
        return Serial.read();
    }

}

#endif
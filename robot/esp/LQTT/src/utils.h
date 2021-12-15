#ifndef UTILS_H
#define UTILS_H

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

#endif
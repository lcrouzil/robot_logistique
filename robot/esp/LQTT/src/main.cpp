#include <Arduino.h>

#include "motor.h"
#include "uart.h"
#include "mqtt.h"

void setup() {
  
  uart::init();
  motor::init();
  mqtt::init();

}

void loop() {
  
  /*uint8_t in = uart::read();

  if(in == uart::STOP) {
    motor::reset();
  }
  else if(in == uart::RESUME) {
    motor::set();
  }*/

  mqtt::test();

  delay(1000);

}
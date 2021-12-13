#include <Arduino.h>
#include <PubSubClient.h>

#include "motor.h"
#include "uart.h"

void setup() {
  
  uart::init();

  motor::init();
  //motor::set();

}

void loop() {
  
  uint8_t in = uart::read();

  if(in == uart::STOP) {
    motor::reset();
  }
  else if(in == uart::RESUME) {
    motor::set();
  }

  delay(100);
}
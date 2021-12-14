#include <Arduino.h>

#include "uart.h"
#include "mqtt.h"

void setup() {
  
  pinMode(D0, OUTPUT);

  uart::init();
  //mqtt::init();

}

void loop() {

  /*mqtt::loop();

  String message = uart::read();
  Serial.println("." + message + ".");
  if(!message.isEmpty())
    mqtt::send(message);

  delay(100);*/

  uart::echo();
  digitalWrite(D0, !digitalRead(D0));

}
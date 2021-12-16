#include <Arduino.h>

#include "uart.h"
#include "mqtt.h"

void setup() {
  
  pinMode(D0, OUTPUT);

  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);

  uart::init();
  mqtt::init();

  Serial.print("\n\n");
}

void loop() {

  mqtt::loop();

  String message = uart::read();
  //Serial.println("." + message + ".");
  if(!message.isEmpty())
    mqtt::send(message);

  delay(100);

  /*digitalWrite(D0, !digitalRead(D0));

  uart::echo();*/
  

}
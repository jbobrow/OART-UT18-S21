/*
 * Serial Port - Hello World
 */

#include "Serial.h"

ServicePortSerial Serial;

void setup() {

  Serial.begin();
  Serial.println("Hello World!");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(buttonPressed()) {
      Serial.println("Button Pressed");
  }
}

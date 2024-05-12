#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  delay(1000); // delay to allow the serial monitor to initialize
  //pinMode(2, OUTPUT);
  Serial.println("Setup!");
}

void loop() {
  Serial.println("Hello, World!");
  delay(1000); // print "Hello, World!" every second

  // Turn on the LED by setting the pin HIGH
  //digitalWrite(2, HIGH);
  //delay(1000); // Wait for 1 second

  // Turn off the LED by setting the pin LOW
  //digitalWrite(2, LOW);
  //delay(1000); // Wait for 1 second
}
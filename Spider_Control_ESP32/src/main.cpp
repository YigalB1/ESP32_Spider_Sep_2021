#include <Arduino.h>
const int red_led_pin = 23;
const int Yellow_led_pin = 22;
const int green_led_pin = 21;

void setup() {
  pinMode(red_led_pin, OUTPUT);
  pinMode(Yellow_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
}

void loop() {
  digitalWrite(red_led_pin, HIGH);  
  digitalWrite(Yellow_led_pin, HIGH);  
  digitalWrite(green_led_pin, HIGH);  
  delay(1000);                  // wait for a second
   digitalWrite(red_led_pin, LOW);  
  digitalWrite(Yellow_led_pin, LOW);  
  digitalWrite(green_led_pin, LOW);  
  delay(1000);
}
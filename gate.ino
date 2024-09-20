#include <ESP8266WiFi.h> // Include the ESP8266 WiFi library
#include <Servo.h>       // Include the Servo library for ESP8266

int chintu = D1; // D8 on NodeMCU corresponds to pin 8
int mintu = D2;   // D7 on NodeMCU corresponds to pin 7

Servo enmotor;      // Create a Servo object
Servo exmotor;

void setup() {
  pinMode(chintu, INPUT);
  pinMode(mintu, INPUT);

  enmotor.attach(D3); // D6 on NodeMCU, corresponds to pin 6, for servo control
  exmotor.attach(D4); // D6 on NodeMCU, corresponds to pin 6, for servo control

}

void loop() {
  if (digitalRead(chintu) == LOW) {
    enmotor.write(85); // Move servo to position 10 degrees
    delay(3000); 
  } else
  {
    enmotor.write(0); // Move servo to position 115 degrees
  }
  if (digitalRead(mintu) == LOW) {
    exmotor.write(85); // Move servo to position 10 degrees
    delay(3000); 
  } else
  {
    exmotor.write(0); // Move servo to position 115 degrees
  }
  

}

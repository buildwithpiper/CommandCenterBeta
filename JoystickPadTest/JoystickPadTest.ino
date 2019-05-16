/*
  Joystick controls the speed of the on-board LED blinking. 
*/

const int yAxis = A0;         // Joystick Y axis
const int ledPin = 13;        // Mouse control LED

void setup() {
  pinMode(ledPin, OUTPUT);    // The LED pin
}

void loop() {
  // Read and scale the two axes:
  int reading =analogRead(A0);
   digitalWrite(ledPin, HIGH);   // Rurn the LED on (HIGH is the voltage level)
   delay(reading);               // Wait for a second
   digitalWrite(ledPin, LOW);    // Turn the LED off by making the voltage LOW
   delay(reading);               // Wait for a second
}


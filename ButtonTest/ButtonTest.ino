/*
  ButtonTest

  Turns on the Aruduino on-board Light Emitting Diode (LED) connected to 
  digital pin 13, when button No.1 (UP) connected to pin 6 is pressed.

*/

// Constants won't change. They're used here to set pin numbers:
const int buttonPin = 6;     // The number of the UP button pin
const int ledPin =  13;      // The number of the LED pin

// Variables will change:
int buttonState = 0;         // A variable for reading the button state

void setup() {
  // Initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // Initialize the button pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Read the state of the button pin:
  buttonState = digitalRead(buttonPin);

  // Check if the button is pressed. If it is, the buttonState is LOW:
  if (buttonState == LOW) {
    // Turn the LED on by making the voltage HIGH:
    digitalWrite(ledPin, HIGH);
  } else {
    // Turn the LED off by making the voltage LOW:
    digitalWrite(ledPin, LOW);
  }
}

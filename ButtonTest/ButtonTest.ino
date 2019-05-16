/*
  Button

  Turns ON the Aruduino on-board light emitting diode(LED) connected to digital pin 13,
  when the button No.1 (UP) connected to pin 6 is pressed

*/

// Constants won't change. They're used here to set pin numbers:
const int buttonPin = 6;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// Variables will change:
int buttonState = 0;         // Variable for reading the pushbutton status

void setup() {
  // Initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // Initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // Check if the pushbutton is pressed. If it is, the buttonState is Low:
  if (buttonState == LOW) {
    // Turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // Turn LED off:
    digitalWrite(ledPin, LOW);
  }
}

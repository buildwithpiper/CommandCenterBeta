/*
  ButtonTest

  Turns on the Aruduino on-board Light Emitting Diode (LED) connected to 
  digital pin 13, when UP button (No.1) connected to pin 6 is pressed.

*/

/////////////////////////////////////////////////////////////////////////
// Pin mappings to Arduino Micro according to the assembly instructions
enum ArduinoPinMap
{
  // Click & Button Pins
  LeftButtonPin = 8,
  RightButtonPin = 9,
  UpButtonPin = 6,
  DownButtonPin = 7,
  LEDPin = 13,
};
/////////////////////////////////////////////////////////////////////////

// This constant sets which button will activate the LED.
// Try changing it to one of the other pins to test the other buttons!
const int selectedButton = UpButtonPin;

// A variable for reading the button state
int buttonState = 0;

void setup()
{
  // Initialize the LED pin as an output:
  pinMode(LEDPin, OUTPUT);
  // Initialize the button pin as an input:
  pinMode(selectedButton, INPUT_PULLUP);
}

void loop()
{
  // Read the state of the button pin:
  buttonState = digitalRead(selectedButton);

  // Check if the button is pressed. If it is, the buttonState is LOW:
  if (buttonState == LOW)
  {
    // Turn the LED on by making the voltage HIGH:
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    // Turn the LED off by making the voltage LOW:
    digitalWrite(LEDPin, LOW);
  }
}

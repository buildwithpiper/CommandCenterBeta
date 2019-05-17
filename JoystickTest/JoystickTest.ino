/*
  Use the joystick to control the speed of the on-board LED blinking.
  Press the joystick down to pause the blinking.
*/

// #define's are used for values defined by the system.
// In this case, we use one to define the maximum value
// that the joystick can output.
#define JOYSTICK_MAX 1024

/////////////////////////////////////////////////////////////////////////
// Pin mappings to Arduino Micro according to the assembly instructions
enum ArduinoPinMap {
  // Joystick Movement
  VerticalAxisPin = A0,
  HorizontalAxisPin = A1,

  // LED & Joystick Press use the same pin
  LEDJoystickPressPin = 13,
};
/////////////////////////////////////////////////////////////////////////

// These constants control the minimum and maximum rate of blinking the LED.
// They specify the number of milliseconds between turning the LED on and off.
// Keep in mind that lower numbers mean faster blinking!
const double maxDelay = 2000; // 2 seconds
const double minDelay = 30;   // 30 milliseconds

// This constant sets which joystick axis will control the LED.
// Try changing it to the other pin to test the other axis!
const int selectedAxis = VerticalAxisPin;

// Variables will change:
int joystickReading;    // A variable for reading the joystick position
int joystickPressState; // A variable for reading the joystick press state
int blinkDelay;         // A variable for storing the blink delay

/*
  Typically we would set the pin modes during setup, but since we need
  to toggle the JoystickPressPin / LEDPin between input and output
  during each loop, and the analog pins don't need to be set at all,
  in this case we can omit the setup function entirely.
*/
void loop() 
{
  // First we set the LEDJoystickPressPin to input mode
  // to check if the joystick is pressed:
  pinMode(LEDJoystickPressPin, INPUT_PULLUP);
  joystickPressState = digitalRead(LEDJoystickPressPin);

  // Only blink the LED if the joystick is not pressed:
  if (joystickPressState != LOW) {
    // Now that we are going to blink the LED, we need to
    // set the LEDJoystickPressPin to output mode:
    pinMode(LEDJoystickPressPin, OUTPUT);

    // Read the value of the joytick y-axis:
    joystickReading = analogRead(selectedAxis);

    // Calculate duration of blink from joystick position:
    blinkDelay = scaleReading(joystickReading, selectedAxis);

    // Turn the LED on by making the voltage HIGH:
    digitalWrite(LEDJoystickPressPin, HIGH);

    // Wait for the duration of blinkDelay:
    delay(blinkDelay);

    // Turn the LED off by making the voltage LOW:
    digitalWrite(LEDJoystickPressPin, LOW);

    // Calculate blink duration again in case the joystick has moved:
    joystickReading = analogRead(selectedAxis);
    blinkDelay = scaleReading(joystickReading, selectedAxis);

    // Wait for the duration of blinkDelay:
    delay(blinkDelay);
  }
}

/*
  Scale the reading from the analog pin to be between minDelay and maxDelay.
  Use a logarithmic scale for a more gradual transition.
*/
int scaleReading(int reading, int axis) {
  // The vertical axis has 0 at the top and JOYSTICK_MAX at the bottom,
  // but the horizontal axis has 0 at the left and JOYSTICK_MAX at the right,
  // so to get consistent behavior with both axes, we reverse the reading
  // for the vertical axis:
  if (axis == VerticalAxisPin) {
    reading = JOYSTICK_MAX - reading;
  }

  // Map the joystick reading, which is JOYSTICK_MAX at the bottom and 0 at the
  // top, to a logarithmic curve from 1 at the bottom to 0 at the top:
  double mappedReading = 1 - log(reading + 1) / log(JOYSTICK_MAX);

  // Scale the mapped reading onto the range
  // from maxDelay at the bottom to minDelay at the top:
  double scaledReading = mappedReading * (maxDelay - minDelay) + minDelay;

  // Return the scaled value:
  return scaledReading;
}

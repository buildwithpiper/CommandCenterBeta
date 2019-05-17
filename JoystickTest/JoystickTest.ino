/*
  Use the joystick to control the speed of the on-board LED blinking. 
*/

// #define's are used for values defined by the system. 
// In this case, we use one to define the maximum value 
// that the joystick can output.
#define JOYSTICK_MAX 1024

// Constants won't change. They're used here to set pin numbers:
const int yAxis = A0;        // The number of the joystick y-axis pin
const int ledPin =  13;      // The number of the LED pin

// These constants control the minimum and maximum rate of blinking the LED.
// They specify the number of milliseconds between turning the LED on and off.
// Keep in mind that lower numbers mean faster blinking!
const double maxDelay = 2000;  // 2 seconds
const double minDelay = 30;    // 30 milliseconds

// Variables will change:
int joystickReading;         // A variable for reading the joystick state
int blinkDelay;              // A variable for storing the blink delay
 
void setup() {
  // Initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Read the value of the joytick y-axis:
  joystickReading = analogRead(yAxis);

  // Calculate duration of blink from joystick position:
  blinkDelay = scaleReading(joystickReading);
  
  // Turn the LED on by making the voltage HIGH:
  digitalWrite(ledPin, HIGH);
   
  // Wait for the duration of blinkDelay:
  delay(blinkDelay);
   
  // Turn the LED off by making the voltage LOW:      
  digitalWrite(ledPin, LOW);    
   
  // Calculate blink duration again in case the joystick has moved:
  joystickReading = analogRead(yAxis);
  blinkDelay = scaleReading(joystickReading);
  
  // Wait for the duration of blinkDelay:
  delay(blinkDelay);
}

/*
  Scale the reading from the analog pin to be between minDelay and maxDelay.
  Use a logarithmic scale for a more gradual transition.
*/
int scaleReading(int reading) {
  // Map the joystick reading, which is JOYSTICK_MAX at the bottom and 0 at the top,
  // to a logarithmic curve from 1 at the bottom to 0 at the top:
  double scaledReading = 1 - log(JOYSTICK_MAX - reading + 1) / log(JOYSTICK_MAX);

  // Use the scaled reading to return a value 
  // from maxDelay at the bottom to minDelay at the top:
  return scaledReading * (maxDelay - minDelay) + minDelay;
}

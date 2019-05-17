// Arduino libraries to control the mouse and keyboard
#include <Mouse.h>
#include <Keyboard.h>

/////////////////////////////////////////////////////////////////////////
// Pin mappings to Arduino Micro according to the assembly instructions
enum ArduinoPinMap {
  VerticalAxisPin   = A0,
  HorizontalAxisPin = A1,

  LeftButtonPin     = 8,
  RightButtonPin    = 9,
  UpButtonPin       = 6,
  DownButtonPin     = 7,
  JoystickPressPin  = 13,
};
/////////////////////////////////////////////////////////////////////////
enum InputAction {
  LeftButton     = 0,
  RightButton,
  UpButton,
  DownButton,
  JoystickLeft,
  JoystickRight,
  JoystickUp,
  JoystickDown
};

int AxisMaxInput = 1023;
int AxisMaxOutput = 8;

int MappedThreshold = 2;              // resting threshold, mapped
int MappedCenter = AxisMaxOutput / 2; // resting position value, mapped

// Reads axis pin and scales the analog input from 0 to <range>
int readAxis(int axis)
{
  int reading = analogRead(axis);
  
  // Map the reading from the analog input range to the output range:
  int mappedReading = map(reading, 0, AxisMaxInput, 0, AxisMaxOutput);
  
  int distanceFromCenter = mappedReading - MappedCenter;
  // If the output reading is outside from the rest position threshold, use it:
  if (abs(distanceFromCenter) < MappedThreshold) {
    distanceFromCenter = 0;
  }
  return distanceFromCenter;
}
int baseDurationLedPulses = 400;
int durationLedPulses = baseDurationLedPulses;
int baseTimeBetweenLedPulses = 400;
int timeBetweenLedPulses = baseTimeBetweenLedPulses;

void joystickActivate(InputAction stickDirection, bool isActive) {
  if (isActive) {
    if (stickDirection == JoystickLeft) {
      // Shorten Pulse Duration
      durationLedPulses = baseDurationLedPulses / 4;
    } else if (stickDirection == JoystickRight) {
      // Lengthen Pulse Duration
      durationLedPulses = baseDurationLedPulses * 2;
    } else if (stickDirection == JoystickUp) {
      // Lengthen Delay Duration
      timeBetweenLedPulses = baseTimeBetweenLedPulses * 4;
    } else if (stickDirection == JoystickDown) {
      // Shorten Delay Duration
      timeBetweenLedPulses = baseTimeBetweenLedPulses / 4;
    }
  }
}

void testJoystick() {
  int horizontalInput = readAxis(HorizontalAxisPin);
  if (horizontalInput > 0) {
    joystickActivate(JoystickRight, abs(horizontalInput) > MappedThreshold);
  } else {
    joystickActivate(JoystickLeft, abs(horizontalInput) > MappedThreshold);
  }
  
  int verticalInput = readAxis(VerticalAxisPin);
  if (verticalInput < 0) {
    joystickActivate(JoystickDown, abs(verticalInput) > MappedThreshold);
  } else {
    joystickActivate(JoystickUp, abs(verticalInput) > MappedThreshold);
  }
}

int checkButtonPress(bool isActive)
{
  if (isActive == true) {
    // Reduce time between LED pulses by 1/4 if button is active
    return baseTimeBetweenLedPulses / 4;
  }
  return 0;
}

void testButtons()
{
  // 0 = 0% reduction in timing (regular)
  // 1 = 25% reduction in delay
  // 2 = 50% reduction in delay
  // 3 = 75% reduction in delay
  // 4 = 100% reduction in delay (solid light)
  timeBetweenLedPulses = baseTimeBetweenLedPulses;
  
  timeBetweenLedPulses -= checkButtonPress(digitalRead(RightButtonPin) == LOW);
  timeBetweenLedPulses -= checkButtonPress(digitalRead(LeftButtonPin) == LOW);
  timeBetweenLedPulses -= checkButtonPress(digitalRead(UpButtonPin) == LOW);
  timeBetweenLedPulses -= checkButtonPress(digitalRead(DownButtonPin) == LOW);
}

void processInput()
{
  testJoystick();
  testButtons();
}

unsigned long timeOfLastAction = 0;
bool ledOn = false;
void tryTogglingLed() 
{
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - timeOfLastAction;
  if (ledOn && elapsedTime >= durationLedPulses && timeBetweenLedPulses > 25) {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    ledOn = false;
    timeOfLastAction = currentTime;
  } else if (!ledOn && elapsedTime >= timeBetweenLedPulses) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    ledOn = true;
    timeOfLastAction = currentTime;
  }
}


// Arduino setup, called in the beginning on the startup
void setup()
{
  // Setup input for all the pins
  pinMode(HorizontalAxisPin, INPUT);
  pinMode(VerticalAxisPin, INPUT);
  pinMode(LeftButtonPin, INPUT_PULLUP);
  pinMode(RightButtonPin, INPUT_PULLUP);
  pinMode(UpButtonPin, INPUT_PULLUP);
  pinMode(DownButtonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

// Main Arduino Program Loop
void loop()
{
  processInput();
  tryTogglingLed();
}

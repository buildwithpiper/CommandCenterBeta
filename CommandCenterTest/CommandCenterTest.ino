/*
 * A very simple Sketch to see that the wiring from the joystick and buttons
 * are connected correctly
 */

/////////////////////////////////////////////////////////////////////////
// Pin mappings to Arduino Micro according to the assembly instructions
enum ArduinoPinMap {
  // Joystick Movement
  VerticalAxisPin = A0,
  HorizontalAxisPin = A1,

  // Click & Button Pins
  LeftButtonPin = 8,
  RightButtonPin = 9,
  UpButtonPin = 6,
  DownButtonPin = 7,
  JoystickPressPin = 13,
};

const char Up[] = "Up";
const char Down[] = "Down";
const char Right[] = "Right";
const char Left[] = "Left";
const char JoystickClick[] = "Click";
const char JoystickUp[] = "^";
const char JoystickDown[] = "v";
const char JoystickLeft[] = "<";
const char JoystickRight[] = ">";

// Convert the joystick values to be centered.
const int AxisMaxInput = 1023;
const int AxisMaxOutput = 16;

// To keep the understanding between this sketch and the others, we map it to a
// threshold, even though it's not strictly necessary.
int MappedThreshold = 3;               // resting threshold, mapped
int MappedCenter = AxisMaxOutput / 2;  // resting position value, mapped


void setup() {
  pinMode(HorizontalAxisPin, INPUT);
  pinMode(VerticalAxisPin, INPUT);
  pinMode(LeftButtonPin, INPUT_PULLUP);
  pinMode(RightButtonPin, INPUT_PULLUP);
  pinMode(UpButtonPin, INPUT_PULLUP);
  pinMode(DownButtonPin, INPUT_PULLUP);
  pinMode(JoystickPressPin, INPUT_PULLUP);
}

void loop() {
  printMessageIfPressed(Up, digitalRead(UpButtonPin) == LOW);
  printMessageIfPressed(Down, digitalRead(DownButtonPin) == LOW);
  printMessageIfPressed(Right, digitalRead(RightButtonPin) == LOW);
  printMessageIfPressed(Left, digitalRead(LeftButtonPin) == LOW);
  printMessageIfPressed(JoystickClick, digitalRead(JoystickPressPin) == LOW);

  int horizontalInput = readAxis(HorizontalAxisPin);
  int verticalInput = readAxis(VerticalAxisPin);
  printMessageIfPressed(JoystickLeft, horizontalInput < -MappedThreshold);
  printMessageIfPressed(JoystickUp, verticalInput < -MappedThreshold);
  printMessageIfPressed(JoystickRight, horizontalInput > MappedThreshold);
  printMessageIfPressed(JoystickDown, verticalInput > MappedThreshold);
}

void printMessageIfPressed(char * message, bool isPressed) {
  if (isPressed) {
    Serial.println(message);
  }
}

int readAxis(int axis) {
  // Read the analog input
  int reading = analogRead(axis);

  // Map the reading from the analog input range to the output range:
  int mappedReading = map(reading, 0, AxisMaxInput, 0, AxisMaxOutput);

  // If the output reading is outside from the rest position threshold, use it:
  int distanceFromCenter = mappedReading - MappedCenter;

  if (abs(distanceFromCenter) < MappedThreshold) {
    distanceFromCenter = 0;
  }

  // Return the distance for this axis:
  return distanceFromCenter;
}

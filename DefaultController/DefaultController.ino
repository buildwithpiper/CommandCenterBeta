// Arduino libraries to control the mouse and keyboard
#include <Mouse.h>
#include <Keyboard.h>

/////////////////////////////////////////////////////////////////////////
// Pin mappings to Arduino Micro according to the assembly instructions
enum ArduinoPinMap {
  // Left is the controller, right is the Arduino Micro pin
  // Joystick Movement
  VerticalAxisPin   = A0,
  HorizontalAxisPin = A1,

  // Click & Button Pins
  LeftButtonPin     = 8,
  RightButtonPin    = 9,
  UpButtonPin       = 6,
  DownButtonPin     = 7,
  JoystickPressPin  = 13,
};
/////////////////////////////////////////////////////////////////////////

// We will use the Joystick press to toggle between these modes
enum ControllerMode {
  // Controller actions drive keyboard presses
  KeyboardControllerMode,
  // Controller actions drive mouse keyboard presses
  MouseAndKeyboardControllerMode,
};

ControllerMode controllerMode = KeyboardControllerMode;
enum InputAction {
  LeftButton     = 0,
  RightButton,
  UpButton,
  DownButton,
  JoystickLeft,
  JoystickRight,
  JoystickUp,
  JoystickDown,
  // We leave out the JoystickPress, since that's a toggle action.
};

// Eight elements to correspond to the button. Each map to a keyboard action
// Change these to map the buttons in Keyboard Mode
const int KeyboardModeMap[8] =
  { 'z',                        // LeftButton
    'c',                        // RightButton
    ' ',                        // UpButton
    'x',                        // DownButton
    KEY_LEFT_ARROW,             // Not applicable
    KEY_RIGHT_ARROW,            // Not applicable
    KEY_UP_ARROW,               // Not applicable
    KEY_DOWN_ARROW,             // Not applicable
  };

// Eight elements to correspond to the button. Each map to a mouse or keyboard
// action. Change these to map the buttons in Mouse/Keyboard Mode
const int MouseAndKeyboardModeMap[8] =
  { ' ',                        // LeftButton
    MOUSE_RIGHT,                // RightButton
    'x',                        // UpButton
    MOUSE_LEFT,                 // DownButton
    KEY_RIGHT_ARROW,            // JoystickRight
    KEY_LEFT_ARROW,             // JoystickLeft
    KEY_UP_ARROW,               // JoystickUp
    KEY_DOWN_ARROW,             // JoystickDown
  };

// This is set on the mode toggle
int * currentModeMap = KeyboardModeMap;
 
const int AxisMaxInput = 1023;
const int AxisMaxOutput = 8;

const uint8_t MOUSE_MOVE_DELAY = 5;
// Global variable for the last time the mouse (aka joystick) was moved
unsigned long lastMouseMovement = 0;

int MappedThreshold = 2;              // resting threshold, mapped
int MappedCenter = AxisMaxOutput / 2; // resting position value, mapped


void toggleControllerMode()
{
  if (controllerMode == KeyboardControllerMode) {
    controllerMode = MouseAndKeyboardControllerMode;
    currentModeMap = MouseAndKeyboardModeMap;
  } else {
    controllerMode = KeyboardControllerMode;
    currentModeMap = KeyboardModeMap;
  }
}

// Reads an axis (data pin) and scales the analog input range to a range
// from 0 to <range>
int readAxis(int axis)
{
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

// Converts a button from the InputAction enum into a mouse or keyboard press
void buttonPress(InputAction button, bool isPress)
{
  int action = currentModeMap[button];

  if ((action == MOUSE_RIGHT) || (action == MOUSE_MIDDLE)
      || (action == MOUSE_LEFT)) {
    if(isPress){
      Mouse.press(action);
    } else {
      Mouse.release(action);
    }
  } else {
    if(isPress) {
      Keyboard.press(action);
    } else {
      Keyboard.release(action);
    }
  }
}

void handleJoystick() {
  if (digitalRead(JoystickPressPin) == LOW) {
    toggleControllerMode();
  }

  int horizontalInput = readAxis(HorizontalAxisPin);
  int verticalInput = readAxis(VerticalAxisPin);
  if (controllerMode == MouseAndKeyboardControllerMode) {
    // Take the horizontal and vertical input and move the mouse
    Mouse.move(horizontalInput, verticalInput, 0);
  } else {
    // Or, convert the horizontal and vertical input and move the mouse
    if (horizontalInput > 0) {
      buttonPress(JoystickRight, abs(horizontalInput) > MappedThreshold);
    } else {
      buttonPress(JoystickLeft, abs(horizontalInput) > MappedThreshold);
    }
    
    if (verticalInput < 0) {
      // This is a little counter-intuitive, but this maps to the controller
      // action
      buttonPress(JoystickUp, abs(verticalInput) > MappedThreshold);
    } else {
      buttonPress(JoystickDown, abs(verticalInput) > MappedThreshold);
    }
  }
}

void handleButtons()
{
  buttonPress(RightButton, digitalRead(RightButtonPin) == LOW);
  buttonPress(LeftButton, digitalRead(LeftButtonPin) == LOW);
  buttonPress(UpButton, digitalRead(UpButtonPin) == LOW);
  buttonPress(DownButton, digitalRead(DownButtonPin) == LOW);
}

// Called from the loop to handle input from the pins. We break up the
// handling into joystick and buttons
void processInput()
{
  unsigned long currentTime = millis();
  if(currentTime - lastMouseMovement >= MOUSE_MOVE_DELAY) {
    // Space out joystick actions. This is especially for the joystick
    // button, which sends multiple events for one press
    handleJoystick();
    lastMouseMovement = currentTime;
  }

  handleButtons();
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
  pinMode(JoystickPressPin, INPUT_PULLUP);
  Mouse.begin();
  // Toggle the mode in the beginning to set the initial values
  toggleControllerMode();
}

// Main Arduino Program Loop
void loop()
{
  // Loop forever, checking the pins for input
  processInput();
}

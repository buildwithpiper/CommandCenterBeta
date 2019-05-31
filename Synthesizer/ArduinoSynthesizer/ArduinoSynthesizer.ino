/*
  Arduino sketch for sending serial commands to generate sound in Processing.
  In Processing Each button would generate a tone with a frequency proportional to the button number.
  The joystick left-right direction shifts the frequency of the tone and up-down direction control the volume. 
*/

/////////////////////////////////////////////////////////////////////////
// Pin mappings to Arduino Micro according to the assembly instructions
enum ArduinoPinMap {
  // Button & LED Pins
  LeftButtonPin = 8,
  RightButtonPin = 9,
  UpButtonPin = 6,
  DownButtonPin = 7,
  LEDPin = 13,
};
/////////////////////////////////////////////////////////////////////////

const int yAxis = A0;                     // Joystick Y axis
const int xAxis = A1;                     // Joystick Y axis

const long JOYSTICK_SAMPLING_PERIOD=100; //Sample joystick every 100ms
const long BUTTON_SAMPLING_PERIOD=200;   //Sample button every 200ms

unsigned long prevJoystickTime=0;
unsigned long prevButtonTime=0;

void setup() 
{
  pinMode(UpButtonPin, INPUT_PULLUP);
  pinMode(LeftButtonPin, INPUT_PULLUP);    
  pinMode(RightButtonPin, INPUT_PULLUP);   
  pinMode(DownButtonPin, INPUT_PULLUP);    
}

void loop() 
{
  unsigned long currentTime = millis();

  //Send data based on the chosen sampling time
  if(currentTime - prevJoystickTime >= JOYSTICK_SAMPLING_PERIOD)
  {
    // Read the two axes:
    int yReading =analogRead(yAxis);
    int xReading =analogRead(xAxis);
    
    //Send joystick measurements to serial
    Serial.print("X");
    Serial.print(xReading);
    Serial.print("e");
    Serial.print("Y");
    Serial.print(yReading);
    Serial.print("e");

    //update last joystick read time
    prevJoystickTime=currentTime;
  }
  
  if (currentTime-prevButtonTime>BUTTON_SAMPLING_PERIOD)
  {
     //Send button press events to serial  
    if(!digitalRead(UpButtonPin))
      Serial.print("B1e");
    else if(!digitalRead(DownButtonPin))
      Serial.print("B2e");
    else if(!digitalRead(LeftButtonPin))
      Serial.print("B3e");
    else if(!digitalRead(RightButtonPin))
      Serial.print("B4e");
      
    //update last button read time
    prevButtonTime=currentTime;
  }
}

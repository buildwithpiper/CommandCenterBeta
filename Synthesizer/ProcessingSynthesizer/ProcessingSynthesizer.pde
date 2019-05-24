/**************
Joystick acts as a musical instrument. Button 1-4 play different tones, with the tone going 
up by a frequency multiple as the button number increases. 

Joystick left & right shift the frequncy around the standard note for the button.
Joystick up & down control the volume. 

The joystick axes readings and button pressses are read from the serial port.
The firmware on the arduino will be generating theis data. 

Serial Command Examples:
"X100e": Joystick horizontal axis reading is 100 (ADC is 10bit so max is 1024)
"Y200e": Joystick vertical axis reading is 200
"B2e":   Button No.2 was pressed
**************/


import processing.sound.*;
import processing.serial.*; 
 
Serial port;                       // Create object from Serial class
Sound s;                           //Create sound

int lf = 'e';                      // Linefeed in ASCII for serial communication


TriOsc pulse = new TriOsc(this);   // Create and start the sine oscillator

int freqMultiple=60;               //Frequency multiple for the button
int freqButttonStart=50;           //Starting frequency for button one
int freqShiftRange=40;             //Shift magnitude introduced by the joystick Left/Right

float minAmp=0.1;                  //Min volume set by joystick Up/Down
float maxAmp=1;                    //Max volume set by joystick Up/Down

PImage freqAmpGraphImage;          //background graph image
float imgXOffset=67.4;             //background graph image X offset
float imgYOffset=62.9;             //background graph image Y offset

float freqShift=0;                 //tone frequency shift
float amplitude=0;                 //sound amplitude

long prevTime=0;                   //last time note was played


void setup() 
{
  size(593,324);                  //Canvas Size, based on background image
  
  // Open the port that the board is connected to and use the same speed (9600 bps)
  // Please select the correct port number below based on the port prints on the debug window below. 
  int PortNumber=1;
  String portName = Serial.list()[PortNumber];
  port = new Serial(this, portName, 9600);
  port.clear();
  // Throw out the first reading, in case we started reading 
  // in the middle of a string from the sender.
  port.readStringUntil(lf);

  // Create a Sound object for globally controlling the output volume.
  s = new Sound(this);
  
  //set background, image should be placed in the 'data' folder in the same directory as this code
  freqAmpGraphImage = loadImage("freqAmpGraph.png");
  background(freqAmpGraphImage);
  //make any drawing fill color red
  fill(255,0,0);
}

void draw() 
{
  String serialString = null;
  //check for serial port incoming data
  while (port.available() > 0) 
  {
    //read input data from serial port until the 'end of line'  character is reached
    serialString = port.readStringUntil(lf);
    //println(serialString);                  //for debug
    if (serialString!=null && serialString.length()>2) 
    {
      char commandType=serialString.charAt(0);
      //the payload data is between the first character, which is the command, 
      //and the 'end of line' character at the end. 
      String payload=serialString.substring(1,serialString.length()-1);
      
      //X axis measurement
      if (commandType=='X')  
      {
        //set the frequency shift for the button note
        freqShift=map(Integer.parseInt(payload), 0, 1024, -freqShiftRange, freqShiftRange);
      }
      //Y axis measurement
      if (commandType=='Y')
      {
        //set the sound amplitude
        int amp=Integer.parseInt(payload);
        amplitude = map(amp, 0, 1024, maxAmp, minAmp);
        s.volume(amplitude);
      }
      
      long currentTime=millis();
      //Button press
      if (commandType=='B') 
      {
        //find what button was pressed 
        int buttonNum=Integer.parseInt(payload);
        //Calculate the button note frequency and apply the frequency shift from joystick
        float freq=buttonNum*freqMultiple+freqShift+freqButttonStart;
        //Canvas Y position for the amplitude
        float yPos=map(amplitude,0,1,264,14);
        
        //update background for note display
        background(freqAmpGraphImage); 
        //draw a cricle of radius 20 at the frequency and amplitude
        circle(freq+imgXOffset,yPos,20); 
        
        //play the note
        prevTime=currentTime;
        pulse.play(freq, 1);
      }
      //Stop the note if it's been played more than 200ms
      if (currentTime-prevTime>200)
      {
        pulse.stop();
      }
    }
  }
}

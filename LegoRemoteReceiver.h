/*
  LegoRemoteReceiver.h - Library for receiving remote commands from Lego IR controls.

***** Usage ********
  
  Hardware: Wire up your IR remote control receiver (half-sphere looking up, description for module mentioned below)
  1) connect the IR middle pin to Arduino GND
  2) connect the IR right pin to Arduino V3.3
  3) connect the IR left pin to Arduino input pin 2
  
  Software:
  
  #include <LegoRemoteReceiver.h>
  
  LegoRemoteReceiver lrr();
  
  void setup() {
    Serial.begin(9600);
    lrr.setInputPin(2);
    lrr.setCallback(actOnCommand);
  }
  
  void loop() {
    lrr.loop();
  }
  
  // your custom logic
  void actOnCommand() {
    if(lrr.getChannel().equals("00") &&
       lrr.getMode().equals("110") &&
       lrr.getData().equals("0100"))
    {
      Serial.println("channel 1, left control wheel clockwise turn");
    }
  }
  
***** Pointers *****

  IR remote control receiver module hardware [1]
  General introduction into decoding IR signals with Arduino [2]
  Lego press release to open source their IR spec [3]
  The Lego IR receiver spec [4]
  
  [1] https://www.adafruit.com/product/157
  [2] https://learn.adafruit.com/ir-sensor/using-an-ir-sensor
  [3] http://www.technicbricks.com/2008/01/power-functions-rc-protocol-released-as.html
  [4] http://storage.technicbricks.com/Media/2008/TBs_20080125_1/LEGO%20Power%20Functions%20RC%20v100.pdf
  
***** Legal ********

  Code released under the MIT License (MIT)
  Copyright (c) 2015, 2016 Matthias Zimmermann
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this 
  software and associated documentation files (the "Software"), to deal in the Software 
  without restriction, including without limitation the rights to use, copy, modify, merge, 
  publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons 
  to whom the Software is furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all copies or 
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
  DEALINGS IN THE SOFTWARE.
*/
#ifndef LegoRemoteReceiver_h
#define LegoRemoteReceiver_h

#include <Arduino.h>

// the port for sampling the ir input pin, update this if you need an input pin other than 0-7
#define LEGO_IR_PORT PIND

// the maximum clicks we'll listen for
#define LEGO_IR_MAXPULSE 200

// what our timing resolution 
#define LEGO_IR_RESOLUTION 40

// class to receive and verifiy lego ir commands
class LegoRemoteReceiver
{
  public:
    // default constructor
    LegoRemoteReceiver();
    
    // set the input pin for the ir sensor (default = 2)
    void setInputPin(int pin);
    
    // set the method to be called if an ir command is received
    void setCallback(void (*commandCallback)());
    
    // wait for ir commands
    void loop();
    
    // channel for the last command: 1="00", 2="01", 3="10", 4="11"
    String getChannel();
    
    // mode for the last command
    String getMode();
    
    // data for the last command
    String getData();
    
  private:
    int _pin;
    void (*_commandCallback)();
    String _command;
    String _lastCommand;
    uint16_t _pulses[100][2]; // pulse storage
    uint8_t _currentpulse; // current pulse index
    
    void _actOnCommand();
    boolean _commandIsValid(String command);
    String _xor(String s1, String s2);
    String _parsePulses(int start);
    int _findStart(int start);
    int _findEnd(int i);
    boolean _inRange(int value, int min_value, int max_value);
    char _mapToBit(int i);

};

#endif

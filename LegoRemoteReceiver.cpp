#include "Arduino.h"
#include "LegoRemoteReceiver.h"

LegoRemoteReceiver::LegoRemoteReceiver() {
  _pin = 2;
  _command = "__________________";
  _lastCommand = "__________________";
  _currentpulse = 0;
}

void LegoRemoteReceiver::setInputPin(int pin) {
  _pin = pin;
}

void LegoRemoteReceiver::setCallback(void (*commandCallback)()) {
  _commandCallback = commandCallback;
}

void LegoRemoteReceiver::loop() {
  uint16_t highpulse, lowpulse; // temporary storage timing
  highpulse = lowpulse = 0; // start out with no pulse length

  // while pin is still HIGH
  while (LEGO_IR_PORT & (1 << _pin)) {
    highpulse++;
    delayMicroseconds(LEGO_IR_RESOLUTION);

    // If the pulse is too long, we 'timed out' - either nothing
    // was received or the code is finished, so print what
    // we've grabbed so far, and then reset
    if ((highpulse >= LEGO_IR_MAXPULSE) && (_currentpulse != 0)) {
      _actOnCommand();
      _currentpulse=0;
      return;
    }
  }
  // we didn't time out so lets stash the reading
  _pulses[_currentpulse][0] = highpulse;

  // same as above for low pin
  while (! (LEGO_IR_PORT & _BV(_pin))) {
    lowpulse++;
    delayMicroseconds(LEGO_IR_RESOLUTION);
    if ((lowpulse >= LEGO_IR_MAXPULSE) && (_currentpulse != 0)) {
      _actOnCommand();
      _currentpulse=0;
      return;
    }
  }
  // we didn't time out so lets stash the reading
  _pulses[_currentpulse][1] = lowpulse;

  // we read one high-low pulse successfully, continue!
  _currentpulse++;
}

void LegoRemoteReceiver::_actOnCommand() {
  _command = _parsePulses(0);
  int pos_end = _command.length();
  int run = 1;
  
  for(; run <= 5 && !_commandIsValid(_command); run++) {
    _command = _parsePulses(pos_end);
    pos_end += _command.length();
  }
  
  if(run <= 5) {
    if(_command.equals(_lastCommand)) {
      return;
    }
    else {
      _lastCommand = _command;
    }
    
    _commandCallback();
  }
}

String LegoRemoteReceiver::getChannel() {
  return _command.substring(3, 5);
}

String LegoRemoteReceiver::getMode() {
  return _command.substring(6, 9);
}

String LegoRemoteReceiver::getData() {
  return _command.substring(9, 13);
}

boolean LegoRemoteReceiver::_commandIsValid(String command) {
  if(command.length() != 18) {
    return false;
  }
  
  if(command.charAt(0) != '<' || command.charAt(17) != '>') {
    return false;
  }
  
  if(command.indexOf('?') >= 0) {
    return false;
  }
  
  command = command.substring(1, 17);
  
  String n1  = command.substring( 0,  4);
  String n2  = command.substring( 4,  8);
  String n3  = command.substring( 8, 12);
  String crc = command.substring(12, 16);
  
  String val = _xor("1111", n1);
  val = _xor(val, n2);
  val = _xor(val, n3);
  
  if(crc.equals(val)) { return true; }
  else                { return false; }
}

// computs the xor for s1 and s2. 
// assumes length 4 for s1 and s2
String LegoRemoteReceiver::_xor(String s1, String s2) {
  String r = "....";
  
  for(int i = 0; i < 4; i++) {
    if(s1.charAt(i) != s2.charAt(i)) { r.setCharAt(i, '1'); }
    else                             { r.setCharAt(i, '0'); }
  }
  
  return r;
}

// transform pulse length sequence into a formatted string
String LegoRemoteReceiver::_parsePulses(int start) {
  int code_start = _findStart(start);
  int code_end = _findEnd(code_start);
  String irCode = "";
  
  if(code_start >= 0 && code_end >= 0) {
    irCode += '<';

    for(int i = code_start; i < code_end; i++) {
      irCode += _mapToBit(i);
    }

    irCode += '>';
  }
  
  return irCode;
}

// searches for start pattern of logitech ir control
int LegoRemoteReceiver::_findStart(int start) {
  int i = start;

  while(i < _currentpulse - 2) {
    if(_inRange(_pulses[i][0], 23-1, 23+1)) {
      return i + 1;
    }

    i++;
  }

  return -1;
}

// searches for end pattern of logitech ir control
int LegoRemoteReceiver::_findEnd(int i) {
  if(i < 0) {
    return -1;
  }

  while(i < _currentpulse) {
    if(_inRange(_pulses[i][0], 23-1, LEGO_IR_MAXPULSE)) {
      return i;
    }

    i++;
  }

  return _currentpulse;
}

boolean LegoRemoteReceiver::_inRange(int value, int min_value, int max_value) {
  if(value >= min_value && value <= max_value) {
    return true;
  }
  
  return false;
}

char LegoRemoteReceiver::_mapToBit(int i) {
  if(_inRange(_pulses[i][0], 5-1, 5+1)) { 
    return '0'; 
  }
  
  if(_inRange(_pulses[i][0], 12-2, 12+2)) { 
    return '1'; 
  }
  
  return '?';
}



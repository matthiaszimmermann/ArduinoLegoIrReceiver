# Arduino LEGO IR Power Functions Receiver

Arduino Library for receiving commands from various Lego IR controllers

## Usage

### Hardware

1. Aruino UNO, breadboard and jumper wires
2. LEGO IR Power Functions IR Remote Control or IR Speed Remote Control [1]
3. IR sensor tuned to 38KHz, eg. a TSOP38238 [2]

Then, wire up your IR remote control receiver (half-sphere looking up)
- connect the IR middle pin to Arduino GND
- connect the IR right pin to Arduino V3.3
- connect the IR left pin to Arduino input pin 2

### Software

Load the ArduinoLegoIrReceiver.ino file into your Arduino IDE:

```
#include "LegoRemoteReceiver.h"

LegoRemoteReceiver lrr;

void setup() {
  Serial.begin(9600);
  Serial.println("ready to receive lego ir commands");
  lrr.setCallback(actOnCommand);
}

void loop() {
  lrr.loop();
}

void actOnCommand() {
  if(lrr.getChannel().equals("00") &&
     lrr.getMode().equals("110") &&
     lrr.getData().equals("0100"))
  {
    Serial.println("channel 1, left control wheel clockwise turn");
  }
  else {
    Serial.print("channel="); Serial.print(lrr.getChannel());
    Serial.print(" mode="); Serial.print(lrr.getMode());
    Serial.print(" data="); Serial.print(lrr.getData());
    Serial.println();
  }
}
```

Change the code in method actOnCommand according to the nees of your project.

## Sources

To build the library information from the following sites have been used:

1. IR remote control receiver module hardware [2]
2. General introduction into decoding IR signals with Arduino [3]
3. Lego press release to open source their IR spec [4]
4. The Lego IR receiver spec [5]
  
## References
[1] http://shop.lego.com/en-US/LEGO-Power-Functions-IR-Speed-Remote-Control-8879<br>
[2] https://www.adafruit.com/products/157<br>
[3] https://learn.adafruit.com/ir-sensor/using-an-ir-sensor<br>
[4] http://www.technicbricks.com/2008/01/power-functions-rc-protocol-released-as.html<br>
[5] http://storage.technicbricks.com/Media/2008/TBs_20080125_1/LEGO%20Power%20Functions%20RC%20v100.pdf<br>

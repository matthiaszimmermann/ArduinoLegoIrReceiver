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


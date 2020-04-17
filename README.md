# TLVCommunication
Arduino library that implements communication via TLV packets

The original intention was to use it for efficient bluetooth communication for different robots (the example sketch does this for the Pololi Zumo robot with the Arduino Uno) but can be used for any serial communication.

The only limitation is that the maximum packet length is 255 and the type field is also a byte, so only 255 values are possible.

#Usage

## Instantiate the object:
#include <TLVReceiver.h>
TLVReceiver tlvr;

## Register the callback
void packetReady(byte type, byte length, byte value[])
{
  Serial.print("type: " + String(type) + ": length: " + String(length)  + " value: ");
  for (int i=0; i<length; ++i)
    {
      Serial.print(value[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
}

void setup() {
  tlvr.setCallback(&packetReady);
}

## Pass bytes from the sender that should respect the TLV format

if(someSerialConnection.available())
    tlvr.addByte(someSerialConnection.read());

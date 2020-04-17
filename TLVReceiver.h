#ifndef TLVRECEIVER_H
#define TLVRECEIVER_H

#include "Arduino.h"

typedef void(*CallbackFunctionPtr)(byte type, byte size, byte value[]);

enum class E_PacketSections : byte
{
  E_Type = 1,
  E_Size = 2,
  E_Value = 3
};

class TLVReceiver
{
public:
    TLVReceiver();
    void addByte(byte b);
    void connectionReset();
    void setCallback(CallbackFunctionPtr p);
private:
    byte currentByte;
    byte currentSize;
    byte currentType;
    E_PacketSections currentSection;
    byte buffer[255];
    void finishedPacket();
    CallbackFunctionPtr ptr;
};

#endif
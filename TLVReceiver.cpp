#include "TLVReceiver.h"

TLVReceiver::TLVReceiver(): currentByte(0)
                          , currentSize(0)
                          , currentType(0)
                          , currentSection(E_PacketSections::E_Type)
{

}

void TLVReceiver::addByte(byte b)
{
    switch(currentSection)
    {
        case E_PacketSections::E_Type:
            currentType = b;
            currentSection = E_PacketSections::E_Size;
            break;
        case E_PacketSections::E_Size:
            currentSize = b;
            if (currentSize == 0)
            {
                finishedPacket();
                currentSection = E_PacketSections::E_Type;
            }
            else
            {
                currentSection = E_PacketSections::E_Value;
                currentByte = 0;
            }
            break;
        case E_PacketSections::E_Value:
            buffer[currentByte] = b;
            ++currentByte;
            if (currentSize == currentByte)
            {
                finishedPacket();
                currentSection = E_PacketSections::E_Type;
            }
            break;
    }
}

void TLVReceiver::connectionReset()
{
    currentByte = 0;
    currentSize = 0;
    currentType = 0;
    currentSection = E_PacketSections::E_Type;
}

void TLVReceiver::finishedPacket()
{
    ptr(currentType, currentSize, buffer);
}

void TLVReceiver::setCallback(CallbackFunctionPtr p)
{
    ptr = p;
}
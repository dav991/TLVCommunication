#include <SoftwareSerial.h>
#include <Wire.h>
#include <ZumoShield.h>

//#define DEBUG 

#include <TLVReceiver.h>

TLVReceiver tlvr;
//Serial connection to a bluetooth module like hc-05
SoftwareSerial btSerial(A4, A5); // RX, TX
ZumoMotors motors;

//union to easily parse date for some packet types
union dualJoystick{
  byte buf[4];
  struct {
    int value1;
    int value2;
  } values;
};

void packetReady(byte type, byte length, byte value[])
{
#ifdef DEBUG
  Serial.print("type: " + String(type) + ": length: " + String(length)  + " value: ");
  for (int i=0; i<length; ++i)
    {
      Serial.print(value[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
#endif
  if (type == 1)
  {
    //single joystick, two bytes expected, the first is power, the second one is angle
    joystickControl(value[0], value[1]);
  }
  else if (type == 2)
  {
    //dual joysticks, both vertical, one for each track of the tank tracks
    union dualJoystick converter;
    for (int i=0; i<4; ++i)
    {
      converter.buf[i] = value[i];
    }
    dualJoystickControl(converter.values.value1, converter.values.value2);
  }
  else if (type == 3)
  {
    union dualJoystick converter;
    for (int i=0; i<4; ++i)
    {
      converter.buf[i] = value[i];
    }
    perpendicularJoystickControl(converter.values.value1, converter.values.value2);
  }
}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  //set the callback function to be called when a packet is complete
  tlvr.setCallback(&packetReady);
  btSerial.begin(9600);
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(btSerial.available())
    tlvr.addByte(btSerial.read());
}

void perpendicularJoystickControl (int leftPower, int rightPower)
{
#ifdef DEBUG
  Serial.println("perpendicularJoystickControl leftPower: " + String(leftPower) + ", rightPower:" + String(rightPower));
#endif
  motors.setRightSpeed((leftPower+rightPower) * 4);
  motors.setLeftSpeed((leftPower-rightPower) * 4);
}

void dualJoystickControl (int leftPower, int rightPower)
{
#ifdef DEBUG
  Serial.println("dualJoystickControl leftPower: " + String(leftPower) + ", rightPower:" + String(rightPower));
#endif
  motors.setRightSpeed(rightPower * 4);
  motors.setLeftSpeed(leftPower * 4);
}

void joystickControl (int power, int angle)
{
  float realAngle = ((float)angle)/40;
#ifdef DEBUG
  Serial.println("joystickControl power: " + String(power) + ", angle:" + String(realAngle) + " sin: " + String(sin(realAngle)) + " cos: " + String(cos(realAngle)));
#endif
  int leftSpeed = sin(realAngle)*power*4 - cos(realAngle) * power * 4;
  int rightSpeed = sin(realAngle)*power*4 + cos(realAngle) * power * 4;
  motors.setRightSpeed(rightSpeed);
  motors.setLeftSpeed(leftSpeed);
}

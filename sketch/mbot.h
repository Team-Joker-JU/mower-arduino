#ifndef MBOT_H
#define MBOT_H

#include <MeAuriga.h>
#include <Wire.h>

MeGyro gyro(0, 0x69);

MeLineFollower lineFonder(PORT_6);

MeEncoderOnBoard rightMotor(SLOT1);
MeEncoderOnBoard leftMotor(SLOT2);

MeUltrasonicSensor ultrasensor(PORT_7);


#endif

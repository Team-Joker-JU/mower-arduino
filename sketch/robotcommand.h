#ifndef ROBOTCOMMAND_H
#define ROBOTCOMMAND_H

#include <stdint.h>

enum RobotCommand : int8_t
{
  HANDSHAKE = 0,
  CONNECTED = 1,
  RECEIVED  = 2,
  ACCELERATION = 3,
  STEERING = 4,
  COLLISION = 5
};

#endif

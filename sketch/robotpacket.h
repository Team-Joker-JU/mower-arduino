#ifndef ROBOTPACKET_H
#define ROBOTPACKET_H

#include "robotcommand.h"

#include <stdint.h>

template <typename T>
class RobotPacket {
  private:
    const int32_t MAX_BUFFER = 64;
    int32_t len;
    RobotCommand command;
    T parameter;
    T from_bytes(int8_t *buffer);
    
  public:
    RobotPacket(RobotCommand command, T parameter);
    RobotPacket(RobotCommand command, T parameter, int32_t len);
    RobotPacket(RobotCommand command, int8_t* buffer, int32_t len);
    RobotCommand get_command();
    T get_parameter();
    void to_bytes(int8_t* buffer);
    int32_t get_length();
};

#endif

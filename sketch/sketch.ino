//#define MBOT

#ifdef MBOT

  #include <MeAuriga.h>
  #include <Wire.h>
  
  MeGyro gyro(0, 0x69);
  
  MeLineFollower lineFonder(PORT_6);
  
  MeEncoderOnBoard rightMotor(SLOT1);
  MeEncoderOnBoard leftMotor(SLOT2);
  
  MeUltrasonicSensor ultrasensor(PORT_7);

#endif


#include "robotpacket.h"

#include <stdint.h>

boolean connected = false;

void setup() {
  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  process_message_protocol();
  delay(10);
}

void on_handshake_completed() {
  connected = true;
  
  /* TODO: Implement some handshake functionality, for example:
   * 
   * - Blink with LEDs
   * - Buzzer noise
  */
}


void on_acceleration_changed(int8_t acceleration) {
  // TODO: Assign value to robot
  Serial.println("on_acceleration_changed triggered"); 
  Serial.println(String(acceleration));
}


void on_steering_changed(int8_t steering) {
  // TODO: Assign value to robot
  Serial.println("on_steering_changed triggered"); 
  Serial.println(String(steering));
}










void read_n_bytes(int8_t* buffer, const int n) {
  unsigned long startTime = millis();
  while ((Serial.available() < n) && (millis() - startTime < 2000)){}
  
  for (size_t i = 0; i < n; i++) {  
    int b = Serial.read();
    if (b < 0) break;   

    *(buffer+i) = static_cast<int8_t>(b);
  }
}


void process_message_protocol() {
  int8_t buffer[64];

  if (Serial.available() > 0) {
    RobotCommand command = static_cast<RobotCommand>(Serial.read());
    
    if (command == HANDSHAKE && !connected) {     
        Serial.write((uint8_t*)HANDSHAKE, sizeof(uint8_t));
        on_handshake_completed();
        
    } else {
      
      switch(command) {
      case ACCELERATION:
        Serial.println("on_acceleration_changed switched"); 
        read_n_bytes(buffer, sizeof(int8_t));
        on_acceleration_changed(RobotPacket<int8_t>(command, buffer).get_parameter());
        break;
      case STEERING:
        Serial.println("on_steering_changed switched"); 
        read_n_bytes(buffer, sizeof(int8_t));
        on_steering_changed(RobotPacket<int8_t>(command, buffer).get_parameter());
        break;
      }
    
      Serial.write((uint8_t*)RECEIVED, sizeof(uint8_t));
    }
  }
}

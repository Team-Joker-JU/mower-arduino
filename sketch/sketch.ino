#include "robotpacket.h"

#include <stdint.h>

#include <MeAuriga.h>
#include <Wire.h>

// For gyroscope
MeGyro gyro(0, 0x69);

// For line follower
MeLineFollower lineFinder(PORT_6);

// For motors
MeEncoderOnBoard rightMotor(SLOT1);
MeEncoderOnBoard leftMotor(SLOT2);

// For UltrasonicSensor
MeUltrasonicSensor ultraSensor(PORT_7);



boolean connected = false;
int8_t current_acceleration;
int8_t current_steering;



void isr_process_encoder1(void)
{
  if (digitalRead(rightMotor.getPortB()) == 0)
    rightMotor.pulsePosMinus();
  else
    rightMotor.pulsePosPlus();
}

void isr_process_encoder2(void)
{ 
  if (digitalRead(leftMotor.getPortB()) == 0)
    leftMotor.pulsePosMinus();
  else
    leftMotor.pulsePosPlus();
}

void setup() {
  Serial.begin(9600);
  Serial.flush();

  gyro.begin();
  attachInterrupt(rightMotor.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(leftMotor.getIntNum(), isr_process_encoder2, RISING);

  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);


  // Start
  on_acceleration_changed(60);
}

void loop() {
  process_message_protocol();
  delay(10);
  
  coordinate_handler(500);
  auto_mode();
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
  
  current_acceleration = acceleration;
  update_motion(current_acceleration, current_steering);
}


void on_steering_changed(int8_t steering) {
  // TODO: Assign value to robot
  Serial.println("on_steering_changed triggered"); 
  Serial.println(String(steering));
  
  current_steering = steering;
  update_motion(current_acceleration, current_steering);
}


bool collision(int distance)
{
  return (ultraSensor.distanceCm() < distance);
}

void update_motion(int8_t acceleration, int8_t steering) {
  float normalized = 1.0f - ((float)abs(steering) / 100.0f);
  int turnAcceleration = (normalized > 0.1f) ? (int)(acceleration * normalized) : acceleration * -1;
  
  // Right turn
  if (steering > 0) {
    rightMotor.setMotorPwm(-turnAcceleration);
    leftMotor.setMotorPwm(acceleration);
  }
  // Left turn
  else if (steering < 0) {
    rightMotor.setMotorPwm(-acceleration);
    leftMotor.setMotorPwm(turnAcceleration);
  }
  // Forward/backwards
  else {
    rightMotor.setMotorPwm(-acceleration);
    leftMotor.setMotorPwm(acceleration);
  }

  rightMotor.updateSpeed();
  leftMotor.updateSpeed();
}





void coordinate_handler(int interval) {
  static unsigned long startTime = millis();
  static float lastX = 0;
  static float lastY = 0;
  static float startRightPosition = rightMotor.getPulsePos();
  static float startLeftPosition = leftMotor.getPulsePos();

  if (isnan(lastX) || isnan(lastY)) {
    lastX = 0;
    lastY = 0;
  }
  
  if (millis() - startTime > interval) {   
    // Update the angle of the gyroscope
    gyro.update();
    float angle = 360 - (gyro.getAngleZ() + 180);
    
    float rightDistance = ((rightMotor.getPulsePos() - startRightPosition) / 360) * 20;
    float leftDistance = ((leftMotor.getPulsePos() - startLeftPosition) / 360) * 20;

    float maxDistance = max(abs(rightDistance), abs(leftDistance));

    float factor = (((-rightDistance + leftDistance) / 2) / maxDistance);
    float distance = maxDistance * factor;

    float x = distance * cos(radians(angle));
    float y = distance * sin(radians(angle));

    lastX = lastX + x;
    lastY = lastY + y;

    Serial.println("------"); 
    Serial.println(String(distance));
    Serial.println(String(angle));
    Serial.println(String(lastX));
    Serial.println(String(lastY));

    startTime = millis();
    startRightPosition = rightMotor.getPulsePos();
    startLeftPosition = leftMotor.getPulsePos();
  }
}


void auto_mode() {
  if (collision(10)) {
    on_acceleration_changed(current_acceleration * -1);
    delay(1000);
    ((random(0, 2) == 0) ? &turn_left : &turn_right)(1000);
    return;
  }

  switch (lineFinder.readSensors()) {
    case S1_IN_S2_IN:
      if (current_acceleration > 0)
        ((random(0, 2) == 0) ? &turn_left : &turn_right)(250);
      break;
    case S1_IN_S2_OUT:
      ((current_acceleration < 0) ? &turn_left : &turn_right)(250);    
      break;
    case S1_OUT_S2_IN:
      ((current_acceleration < 0) ? &turn_right : &turn_left)(250);           
      break;
    case S1_OUT_S2_OUT:
      break;
  }
}

void turn_right(int duration) {
  on_steering_changed(100);
  delay(duration);
  on_steering_changed(0);
}

void turn_left(int duration) {
  on_steering_changed(-100);
  delay(duration);
  on_steering_changed(0);
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

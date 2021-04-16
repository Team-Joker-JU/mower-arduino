#include <MeAuriga.h>

// For line
MeLineFollower lineFinder(PORT_6);

// For motor
MeEncoderOnBoard rightMotor(SLOT1);
MeEncoderOnBoard leftMotor(SLOT2);

// For UltrasonicSensor
MeUltrasonicSensor ultraSensor(PORT_7);

// Global direction
enum DIRECTION { FORWARD, BACK, LEFT, RIGHT };

DIRECTION direction = BACK;
int SPEED = 50;

void isr_process_encoder1(void){
  if (digitalRead(rightMotor.getPortB()) == 0){
    rightMotor.pulsePosMinus();
  }
  else {
    rightMotor.pulsePosPlus();
  }
}

void isr_process_encoder2(void)
{
  if(digitalRead(leftMotor.getPortB()) == 0)
  {
    leftMotor.pulsePosMinus();
  }
  else
  {
    leftMotor.pulsePosPlus();
  }
}

void setup() {
  Serial.begin(115200);
  attachInterrupt(rightMotor.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(leftMotor.getIntNum(), isr_process_encoder2, RISING);

  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}



// OM VÄNSTER ÄR NEGATIV OCH HÖGER POSITIV SÅ GÅR DEN BAKÅT
// 1 är HÖGER
// 2 är VÄNSTER
void driveForward(){
  if (ultraSensor.distanceCm() < 20 ){
    motorStop();
  }
  else{
    leftMotor.setMotorPwm(SPEED);
    rightMotor.setMotorPwm(-SPEED);
    leftMotor.updateSpeed();
    rightMotor.updateSpeed();
    direction = FORWARD;
  }
}

void driveBack(){
   if (ultraSensor.distanceCm() < 20 ){
    motorStop();
  }
  else{
    leftMotor.setMotorPwm(-SPEED);
    rightMotor.setMotorPwm(SPEED);
    leftMotor.updateSpeed();
    rightMotor.updateSpeed();
    direction = BACK;
    }
}
/*
void changeDirectionRight(){
  // Only drive with left motor to turn
  leftMotor.setMotorPwm(-SPEED);
  rightMotor.setMotorPwm(0);
  rightMotor.updateSpeed();
  leftMotor.updateSpeed();
  delay(1000);
  driveForward();
}
void changeDirectionLeft(){
  // Only drive with left motor to turn
  leftMotor.setMotorPwm(0);
  rightMotor.setMotorPwm(SPEED);
  rightMotor.updateSpeed();
  leftMotor.updateSpeed();
  delay(1000);
  driveForward();
}
*/
void driveRight(){
  if (direction == BACK) {
    driveForward();
    delay(100);
    rightMotor.setMotorPwm(SPEED);
    rightMotor.updateSpeed();
    leftMotor.setMotorPwm(SPEED);
    leftMotor.updateSpeed();
  } else {
    rightMotor.setMotorPwm(-SPEED);
    rightMotor.updateSpeed();
    leftMotor.setMotorPwm(-SPEED);
    leftMotor.updateSpeed();
  }
  delay(500);
  driveForward();
}


void driveLeft(){
  if (direction == BACK){
    driveForward();
    delay(100);
    rightMotor.setMotorPwm(-SPEED);
    rightMotor.updateSpeed();
    leftMotor.setMotorPwm(-SPEED);
    leftMotor.updateSpeed();
    } else {
      rightMotor.setMotorPwm(SPEED);
      rightMotor.updateSpeed();
      leftMotor.setMotorPwm(SPEED);
      leftMotor.updateSpeed();
    }
    delay(500);
    driveForward();
}
void motorStop(){
    rightMotor.setMotorPwm(0);
    rightMotor.updateSpeed();
    leftMotor.setMotorPwm(0);
    leftMotor.updateSpeed();
    Serial.println("ultrasonic");
    Serial.println(ultraSensor.distanceCm());
    delay(1000);
}

int startdir = 1;
static char data;
void loop() {
  if (Serial.available()){
    data = Serial.read();
    Serial.readString();
    Serial.println(data);
  }
  switch(data){
    case '1':
      driveForward();
      Serial.println("HEEEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLLOO");
      break;
    case '2':
      motorStop();
      Serial.println("ALOOOOOOOOOOOOOOOHA");
      delay(5000);
      break;
    default: 
      break;
  }

  if(startdir == 1){
    driveForward();
    startdir += 1;
  }
  
  // put your main code here, to run repeatedly:
    
  
  int sensorState = lineFinder.readSensors();
  switch(sensorState) {
    case S1_IN_S2_IN: 
      Serial.println("Båda inne"); 
      Serial.write("båda inne");
      if (direction == FORWARD) {
        //driveBack();
        int randomInt = random(1,3);
        if(randomInt==1){
          rightMotor.setMotorPwm(-SPEED);
          rightMotor.updateSpeed();
          leftMotor.setMotorPwm(-SPEED);
          leftMotor.updateSpeed();
        }
        else if(randomInt==2){
          rightMotor.setMotorPwm(SPEED);
          rightMotor.updateSpeed();
          leftMotor.setMotorPwm(SPEED);
          leftMotor.updateSpeed();
        }
        int randomDelay = random(1000, 2001);
        delay(randomDelay);
      }
      else {
        driveForward();
        delay(500);
      }
      break;
    case S1_IN_S2_OUT: 
      Serial.println("Vänster inne");
      Serial.write("Vänster inne");
      if (direction == BACK) {
        driveRight(); 
      } else {
        //changeDirectionLeft();  
        driveLeft();
      }
      
      break;
    case S1_OUT_S2_IN: 
      Serial.println("Höger inne");
      Serial.write("Höger inne");
      if (direction == BACK) {
        driveLeft();
      } else {
        //changeDirectionRight();
        driveRight();
          
      }
      
      break;
    case S1_OUT_S2_OUT: 
      Serial.println("Båda ute"); 
      Serial.write("Båda ute");
      if (direction == FORWARD) {
        driveForward();
      } else {
        driveBack();
      }
      break;
    default: break;
  }
  delay(50);
}

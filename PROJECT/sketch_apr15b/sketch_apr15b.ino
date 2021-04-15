#include <MeAuriga.h>

// For line
MeLineFollower lineFinder(PORT_6);

// For motor
MeEncoderOnBoard rightMotor(SLOT1);
MeEncoderOnBoard leftMotor(SLOT2);

// ARVIN HAR ÄNDRAT

// Global direction
enum DIRECTION { FORWARD, BACK, LEFT, RIGHT };

DIRECTION direction = BACK;
int SPEED = 170;

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
  leftMotor.setMotorPwm(SPEED);
  rightMotor.setMotorPwm(-SPEED);
  leftMotor.updateSpeed();
  rightMotor.updateSpeed();
  direction = FORWARD;
}

void driveBack(int speed){
  leftMotor.setMotorPwm(-speed);
  rightMotor.setMotorPwm(speed);
  leftMotor.updateSpeed();
  rightMotor.updateSpeed();
  direction = BACK;
}

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

void driveRight(){
  if (direction == BACK) {
    rightMotor.setMotorPwm(-SPEED+40);
    rightMotor.updateSpeed();
    leftMotor.setMotorPwm(SPEED);
    leftMotor.updateSpeed();
  } else {
    rightMotor.setMotorPwm(-SPEED);
    rightMotor.updateSpeed();
    leftMotor.setMotorPwm(0);
    leftMotor.updateSpeed();
  }

  delay(1000);
  driveForward();
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorState = lineFinder.readSensors();
  switch(sensorState) {
    case S1_IN_S2_IN: 
      Serial.println("Båda inne"); 
      if (direction == FORWARD) {
        driveBack(SPEED);
        delay(500);
      } else if ( direction == BACK ) {
        driveForward();
        delay(500);
      }
      break;
    case S1_IN_S2_OUT: 
      Serial.println("Vänster inne");
      if (direction == BACK) {
        driveRight(); 
      } else {
        changeDirectionLeft();  
      }
      
      break;
    case S1_OUT_S2_IN: 
      Serial.println("Höger inne");
      if (direction == BACK) {
        driveForward();
      } else {
        changeDirectionRight();  
      }
      
      break;
    case S1_OUT_S2_OUT: 
      Serial.println("Båda ute"); 
      if (direction == FORWARD) {
        driveForward();
      } else if ( direction == BACK ) {
        driveBack(SPEED);
      }
      break;
    default: break;
  }
  delay(100);
}

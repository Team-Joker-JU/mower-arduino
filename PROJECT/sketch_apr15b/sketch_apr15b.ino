#include <MeAuriga.h>

// For line
MeLineFollower lineFinder(PORT_6);

// For motor
MeEncoderOnBoard rightMotor(SLOT1);
MeEncoderOnBoard leftMotor(SLOT2);

// Global direction
enum DIRECTION { FORWARD, BACK, LEFT, RIGHT };

DIRECTION direction = BACK;

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
void driveForward(int speed){
  leftMotor.setMotorPwm(speed);
  rightMotor.setMotorPwm(-speed);
  leftMotor.updateSpeed();
  rightMotor.updateSpeed();
}

void driveBack(int speed){
  leftMotor.setMotorPwm(-speed);
  rightMotor.setMotorPwm(speed);
  leftMotor.updateSpeed();
  rightMotor.updateSpeed();
}

void changeDirectionRight(){
  // Only drive with left motor to turn
  leftMotor.setMotorPwm(-130);
  rightMotor.setMotorPwm(0);
  rightMotor.updateSpeed();
  leftMotor.updateSpeed();
  delay(1000);
  driveForward(130);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorState = lineFinder.readSensors();
  switch(sensorState) {
    case S1_IN_S2_IN: 
      Serial.println("Båda inne"); 
      if (direction == FORWARD) {
        driveBack(130);
        delay(500);
        direction = BACK;
      } else if ( direction == BACK ) {
        driveForward(130);
        delay(500);
        direction = FORWARD;
      }
      break;
    case S1_IN_S2_OUT: 
      Serial.println("Vänster inne"); 
      
      rightMotor.setMotorPwm(0);
      leftMotor.setMotorPwm(0);
      rightMotor.updateSpeed();
      leftMotor.updateSpeed();
      break;
    case S1_OUT_S2_IN: 
      Serial.println("Höger inne"); 
      changeDirectionRight();
      break;
    case S1_OUT_S2_OUT: 
      Serial.println("Båda ute, Kör framåt!"); 
      if (direction == FORWARD) {
        driveForward(130);
        direction = FORWARD;
      } else if ( direction == BACK ) {
        driveBack(130);
        direction = BACK;
      }
      break;
    default: break;
  }
  delay(100);
}

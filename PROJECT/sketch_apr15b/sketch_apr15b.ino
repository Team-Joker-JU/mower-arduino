#include <MeAuriga.h>
#include <Wire.h>

MeGyro gyro(0,0x69);

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
float previousX=0;
float previousY=0;
int startdir = 1;
char data;
bool manual = true;

long pulse =0;
long pulse2 = 0;
float distanceX=0;
float distanceY=0;


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
  gyro.begin();
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

  //leftMotor.setMotorPwm(SPEED);
  //rightMotor.setMotorPwm(-SPEED);
  leftMotor.setMotorPwm(60);
  rightMotor.setMotorPwm(-60);
  leftMotor.updateSpeed();
  rightMotor.updateSpeed();
  direction = FORWARD;
}

void driveBack(){
  
  //leftMotor.setMotorPwm(-SPEED);
  //rightMotor.setMotorPwm(SPEED);
  leftMotor.setMotorPwm(-60);
  rightMotor.setMotorPwm(60);
  leftMotor.updateSpeed();
  rightMotor.updateSpeed();
  direction = BACK;
}

void collisionDetection(){
  if(ultraSensor.distanceCm() < 10){
    Serial.write("obstacle detected, chainging direction");
    motorStop();
    int randomInt = random(1,3);
    if(randomInt==1){
      driveLeft();
    }
    else if(randomInt==2){
      driveRight();
    }
    
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
    //rightMotor.setMotorPwm(SPEED);
    rightMotor.setMotorPwm(60);
    rightMotor.updateSpeed();
    //leftMotor.setMotorPwm(SPEED);
    leftMotor.setMotorPwm(60);
    leftMotor.updateSpeed();
  } else {
    //rightMotor.setMotorPwm(-SPEED);
    rightMotor.setMotorPwm(-SPEED);
    rightMotor.updateSpeed();
    //leftMotor.setMotorPwm(-SPEED);
    
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
    //Serial.println("ultrasonic");
    Serial.println(ultraSensor.distanceCm());
    delay(500);
}

void manualDriveForward(){
  
    leftMotor.setMotorPwm(SPEED);
    rightMotor.setMotorPwm(-SPEED);
    leftMotor.updateSpeed();
    rightMotor.updateSpeed();
  
}

void manualDriveBackwards(){
    leftMotor.setMotorPwm(-SPEED);
    rightMotor.setMotorPwm(SPEED);
    leftMotor.updateSpeed();
    rightMotor.updateSpeed();
}

void manualDriveLeft(){
  
    rightMotor.setMotorPwm(-SPEED);
    rightMotor.updateSpeed();
    leftMotor.setMotorPwm(-SPEED);
    leftMotor.updateSpeed();
    
}

void manualDriveRight(){
  
      rightMotor.setMotorPwm(SPEED);
      rightMotor.updateSpeed();
      leftMotor.setMotorPwm(SPEED);
      leftMotor.updateSpeed(); 
}

void updatePosition(float angle, int pulseRight, int pulseLeft){
  
  int averagePulses=(pulseLeft + (-pulseRight))/2;
  distanceX=-averagePulses*0.056694*sin(angle*(3.14/180))+previousX;
  distanceY=-averagePulses*0.056694*cos(angle*(3.14/180))+previousY;
  leftMotor.setPulsePos(0);
  rightMotor.setPulsePos(0);
  previousX=distanceX;
  previousY=distanceY;
  Serial.print("(");
  Serial.print(distanceX);
  Serial.print(", ");
  Serial.print(distanceY);
  Serial.println(")");
}




void loop() {
  gyro.update();
  delay(10);
  data = ' ';
  if (Serial.available()){
    data = Serial.read();
    Serial.println(data);
  }
  

   if (data == '6' || manual == true){
    switch(data){
      case '1':
        manualDriveForward();
        Serial.println("HEEEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLLOO");
        Serial.write("Driving Forwards");
        delay(50);
        break;
      case '2':
        Serial.write("Turning left");
        manualDriveLeft();
        delay(50);
        leftMotor.setPulsePos(0);
        rightMotor.setPulsePos(0);
        break;
      case '3':

        Serial.write("Turning right");
        manualDriveRight();
        delay(50);
        leftMotor.setPulsePos(0);
        rightMotor.setPulsePos(0);
      break; 
  
      case '4':

        // long pulses = leftMotor.getPulsePos();
        Serial.write("Driving Backwards");
        manualDriveBackwards();
        delay(50);
      
      break;
  
      case '5':
      int leftPulses = leftMotor.getPulsePos();
      int rightPulses = rightMotor.getPulsePos();
      updatePosition(gyro.getAngleZ(), leftPulses, rightPulses);
      /*
      Serial.read();
      pulse2 = leftMotor.getPulsePos();
      Serial.println("Left motor pulses");
      Serial.println(pulse2);
      Serial.println("Left motor cm: ");
      Serial.println(pulse2*0.056694);
      distanceX=pulse2*0.056694*sin(gyro.getAngleZ()*(3.14/180));
      Serial.println("distance x: ");
      Serial.println(distanceX);
      Serial.println("Angle:");
      Serial.println(gyro.getAngleZ());
      
      
      pulse2 = rightMotor.getPulsePos();
      Serial.println("Right motor pulses");
      Serial.println(pulse2);
      Serial.println("Right motor cm: ");
      Serial.println(pulse2*0.056694);
      distanceY=-pulse2*0.056694*cos(gyro.getAngleZ()*(3.14/180));
      Serial.println("Angle:");
      Serial.println(gyro.getAngleZ());
      Serial.println("distance y: ");
      Serial.println(distanceY);
      gyro.update();
      delay(10);
      */
      
      leftMotor.setPulsePos(pulse);
      rightMotor.setPulsePos(pulse);
      Serial.write("breaking");
      motorStop();
      delay(50);
      break; 
  
      case '6':
      Serial.write("switcing mode");
      if (manual == false){
        manual = true;
        Serial.write("MANUAL!!!!!!!!!!!!!!!");
      }
      else{
        manual = false;
        Serial.write("AUTONOMOUS!!!!!!!!!!!!!");
      }
      break;
      
      default: 
        break;
    }
  }
  if (manual == false){ //everything below is autonimus
    if(startdir == 1){
      driveForward();
      startdir += 1;
    }
    collisionDetection();
    
    // put your main code here, to run repeatedly:
      
    
    int sensorState = lineFinder.readSensors();
    switch(sensorState) {
      case S1_IN_S2_IN: 
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
        if (direction == BACK) {
          driveRight(); 
        } else {
          //changeDirectionLeft();  
          driveLeft();
        }
        
        break;
      case S1_OUT_S2_IN: 
        if (direction == BACK) {
          driveLeft();
        } else {
          //changeDirectionRight();
          driveRight();
            
        }
        
        break;
      case S1_OUT_S2_OUT: 
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

}

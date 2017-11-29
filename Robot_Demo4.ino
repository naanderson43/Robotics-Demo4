/*  Robot arm control program for
 *  ECE 5397: Introduction to Robotics
 *  University of Houston, Fall 2017
 *  Authors: Nick Anderson & Sikender Shahid
 *  Course Instructor: Dr. Aaron Becker
 */


//-------------------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------------------

// Motor enable pins
int enGripper = 2;       //Enable gripper motor
int enWrist = 3;         //Enable wrist motor
int enElbow = 4;        //Enable elbow motor
int enShoulderLift = 5; //Enable shoulder lift motor
int enShoulderRot = 7;  //Enable shoulder rotation motor
  
// Motor control pins
int gripperOpen = 26, gripperClose = 24;        //Gripper Pins
int wristUp = 30, wristDown = 28;               //Wrist Pins
int elbowUp = 37, elbowDown = 35;               //Elbow Pins
int shoulderUp = 39, shoulderDown = 41;         //Shoulder Lift Pins
int shoulderRight = 50, shoulderLeft = 48;      //Shoulder Rotate Pins


//-------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------
#define OPEN 1
#define CLOSE 2
#define UP 3
#define DOWN 4
#define FWD 5
#define BCK 6
#define RIGHT 7
#define LEFT 8


//-------------------------------------------------------------------------------
// Prototypes
//-------------------------------------------------------------------------------
void Gripper(int dir, int pw);
void Wrist(int dir, int pw);
void Elbow(int dir, int pw);
void Shoulder(int dir, int pw);
void CamUp(void);
void CamDown(void);
void CamLeft(void);
void CamRight(void);
void LiftStop(void);
void RotateStop(void);


//-------------------------------------------------------------------------------
// setup()
//
// Configures digital pins, clears outputs,
// and initializes serial port
//-------------------------------------------------------------------------------
void setup() {

  // Set enable pins as outputs
  pinMode(enShoulderLift, OUTPUT);
  pinMode(enElbow, OUTPUT);
  pinMode(enWrist, OUTPUT);
  pinMode(enGripper, OUTPUT);
  pinMode(enShoulderRot, OUTPUT);

  // Set motor pins as outputs
  pinMode(gripperOpen, OUTPUT);
  pinMode(gripperClose, OUTPUT);
  pinMode(wristUp, OUTPUT);
  pinMode(wristDown, OUTPUT);
  pinMode(elbowUp, OUTPUT);
  pinMode(elbowDown, OUTPUT);
  pinMode(shoulderUp, OUTPUT);
  pinMode(shoulderDown, OUTPUT);
  pinMode(shoulderRight, OUTPUT);
  pinMode(shoulderLeft, OUTPUT);

  // Set all outputs low
  clearOuts();

  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while(!Serial){} // Wait for serial port to connect
}

//-------------------------------------------------------------------------------
// loop()
//
// Runs control program
//-------------------------------------------------------------------------------
void loop(){
  static int i = 0;
  int j;
  static char inStr[4];
  char c;

  while(Serial.available() > 0){
    c = Serial.read();
    inStr[i] = c;

    if(c == 13){
      inStr[i] = '\0';
      
      // Commands used in MatLAB program
      if(!strcmp(inStr, "CO")){clearOuts();}
      if(!strcmp(inStr, "LS")){LiftStop();}
      if(!strcmp(inStr, "RS")){RotateStop();}
      if(!strcmp(inStr, "UP")){CamUp();}
      if(!strcmp(inStr, "DN")){CamDown();}
      if(!strcmp(inStr, "LT")){CamLeft();}
      if(!strcmp(inStr, "RT")){CamRight();}
      
      // Joint control cammands used for testing and PWM calibration
      if(!strcmp(inStr, "SR")){Shoulder(RIGHT, 95);}
      if(!strcmp(inStr, "SL")){Shoulder(LEFT, 95);}
      if(!strcmp(inStr, "SF")){Shoulder(FWD,45);}
      if(!strcmp(inStr, "SB")){Shoulder(BCK, 50);}
      if(!strcmp(inStr, "EU")){Elbow(UP, 135);}
      if(!strcmp(inStr, "ED")){Elbow(DOWN, 135);}
      if(!strcmp(inStr, "WU")){Wrist(UP, 140);}
      if(!strcmp(inStr, "WD")){Wrist(DOWN, 140);}
      if(!strcmp(inStr, "GO")){Gripper(OPEN, 40);}
      if(!strcmp(inStr, "GC")){Gripper(CLOSE, 40);}
      
      for(j = 0; j < (sizeof(inStr)/(sizeof(inStr[0]))); j++){
        inStr[j] = NULL;
      }

      i = 0;
    }
    else{i++;}
  }
}

//-------------------------------------------------------------------------------
// Gripper()
//
// Opens or closes gripper
//-------------------------------------------------------------------------------
void Gripper(int dir, int pw){
  if(dir == OPEN){
    digitalWrite(gripperOpen, HIGH);
    digitalWrite(gripperClose, LOW);
    analogWrite(enGripper, pw);
  }

  if(dir == CLOSE){
    digitalWrite(gripperOpen, LOW);
    digitalWrite(gripperClose, HIGH);
    analogWrite(enGripper, pw);
  }
}

//-------------------------------------------------------------------------------
// Wrist()
//
// Raises or lowers wrist
//-------------------------------------------------------------------------------
void Wrist(int dir, int pw){
  if(dir == UP){
    digitalWrite(wristUp, HIGH);
    digitalWrite(wristDown, LOW);
    analogWrite(enWrist, pw);
  }

  if(dir == DOWN){
    digitalWrite(wristUp, LOW);
    digitalWrite(wristDown, HIGH);
    analogWrite(enWrist, pw);
  }
}

//-------------------------------------------------------------------------------
// Elbow()
//
// Raises or lowers elbow
//-------------------------------------------------------------------------------
void Elbow(int dir, int pw){
  if(dir == UP){
    digitalWrite(elbowUp, HIGH);
    digitalWrite(elbowDown, LOW);
    analogWrite(enElbow, pw);
  }

  if(dir == DOWN){
    digitalWrite(elbowUp, LOW);
    digitalWrite(elbowDown, HIGH);
    analogWrite(enElbow, pw);
  }
}

//-------------------------------------------------------------------------------
// Shoulder()
//
// Raises, lowers, or rotates shoulder
//-------------------------------------------------------------------------------
void Shoulder(int dir, int pw){
  if(dir == RIGHT){
    digitalWrite(shoulderRight, HIGH);
    digitalWrite(shoulderLeft, LOW);
    analogWrite(enShoulderRot, pw);
  }

  if(dir == LEFT){
    digitalWrite(shoulderRight, LOW);
    digitalWrite(shoulderLeft, HIGH);
    analogWrite(enShoulderRot, pw);
  }

  if(dir == FWD){
    digitalWrite(shoulderUp, HIGH);
    digitalWrite(shoulderDown, LOW);
    analogWrite(enShoulderLift, pw);
  }

  if(dir == BCK){
    digitalWrite(shoulderUp, LOW);
    digitalWrite(shoulderDown, HIGH);
    analogWrite(enShoulderLift, pw);
  }
}

//-------------------------------------------------------------------------------
// CamUp()
//
// Raises camera
//-------------------------------------------------------------------------------
void CamUp(void){
  Wrist(DOWN, 80);
  Elbow(UP, 150);
}

//-------------------------------------------------------------------------------
// CamDown()
//
// Lowers camera
//-------------------------------------------------------------------------------
void CamDown(void){
  Wrist(UP, 100);
  Elbow(DOWN, 110);
}

//-------------------------------------------------------------------------------
// LiftStop()
//
// Stops camera up/down movement
//-------------------------------------------------------------------------------
void LiftStop(void){
  digitalWrite(enWrist, LOW);
  digitalWrite(enElbow, LOW);
}

//-------------------------------------------------------------------------------
// CamLeft()
//
// Rotates camera left
//-------------------------------------------------------------------------------
void CamLeft(void){
  Shoulder(LEFT, 100);
}

//-------------------------------------------------------------------------------
// CamRight()
//
// Rotates camera right
//-------------------------------------------------------------------------------
void CamRight(void){
  Shoulder(RIGHT, 100);
}

//-------------------------------------------------------------------------------
// RotateStop()
//
// Stops camera left/right movement
//-------------------------------------------------------------------------------
void RotateStop(void){
  digitalWrite(enShoulderRot, LOW);
}

//-------------------------------------------------------------------------------
// clearOuts()
//
// Sets all digital output pins to LOW
//-------------------------------------------------------------------------------
void clearOuts(){
  digitalWrite(enShoulderLift, LOW);
  digitalWrite(enElbow, LOW);
  digitalWrite(enWrist, LOW);
  digitalWrite(enGripper, LOW);
  digitalWrite(enShoulderRot, LOW);
  digitalWrite(gripperOpen, LOW);
  digitalWrite(gripperClose, LOW);
  digitalWrite(wristUp, LOW);
  digitalWrite(wristDown, LOW);
  digitalWrite(elbowUp, LOW);
  digitalWrite(elbowDown, LOW);
  digitalWrite(shoulderUp, LOW);
  digitalWrite(shoulderDown, LOW);
  digitalWrite(shoulderRight, LOW);
  digitalWrite(shoulderLeft, LOW);
}

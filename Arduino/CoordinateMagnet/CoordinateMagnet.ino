#include <AccelStepper.h>
#include <ezButton.h>


// Define pin connections
const int dirPin = 5;
const int stepPin = 2;
const int dirPin2 =6;
const int stepPin2 = 3;
const int magnet = 11;

//define limit switches

ezButton limitswitch(10);
ezButton limitswitch2(9);


// Define two steppers and the pins they will use
AccelStepper stepper1(AccelStepper::DRIVER, stepPin, dirPin);
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);


// Declare variables for coordinates
float coordinateOne;    //Declare a float variable to hold your height
float coordinateTwo;
float coordinateThree;
float coordinateFour;

// Calibration
float calibration = 1; 
 
void setup() {
  // Initialize Serial Port
  Serial.begin(9600);     

  // Stepper Motor Settings
  stepper1.setMaxSpeed(2000);
  stepper1.setAcceleration(2200);
  stepper2.setMaxSpeed(2000);
  stepper2.setAcceleration(2200);
  float pos1 = 200;
  float pos2 = 200;

  // Limit Switch Settings 
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  limitswitch.setDebounceTime(5); // set debounce time to 50 milliseconds
  limitswitch2.setDebounceTime(5);

  // Electromagnet Pin Setup 
  pinMode(magnet, OUTPUT); 
}
 
void loop() {
  //Recieve input for the coordinates
  coordinateOneInput();
  coordinateTwoInput();
  coordinateThreeInput();
  coordinateFourInput();


  
  int flag1 = 1;
  int flag2 = 1;
  float pos1 = 200;
  float pos2 = 200;
  float pos3 = 200;
  float pos4 = 200; 
  float pos5 = 200;
  float pos6 = 200; 
  float pos7 = 200;
  float pos8 = 200;  
  

   float rotationOne = coordinateOne * calibration;
   float rotationTwo = coordinateTwo * calibration;


   // Moves electromagnet to desired x coordinate
   magnetOn();
   if (coordinateOne >= 0){
    pos1 = -pos1 * rotationOne;
    pos2 = pos2 * rotationOne;    
    while (flag1 == 1 && flag2 == 1){
      
      stepper1.moveTo(pos1);

      stepper2.moveTo(pos2);

      stepper1.run();
      stepper2.run();

      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;
   }

  
  // Moves electromagnet to desired y coordinate
  if (coordinateTwo >= 0){
    int temp3 = -pos3 * rotationTwo;
    int temp4 = -pos4 * rotationTwo;
    Serial.println(temp3);
    Serial.println(temp4);
    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(temp3);
      stepper2.moveTo(temp4);

      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }  
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;

  }



  // Turns on electromagnet
  magnetOn();
  delay(500);
  // New Coordinate Math

  float x2 = coordinateThree - coordinateOne;
  float y2 = coordinateFour - coordinateTwo; 
  float rotationThree = x2 * calibration;
  float rotationFour = y2 * calibration;

  // Moves Piece to new x-coordinate
  if (coordinateThree >= 0){
    float temp5 = -pos5 * rotationThree;
    float temp6 = pos6 * rotationThree;    
    while (flag1 == 1 && flag2 == 1){
      
      stepper1.moveTo(temp5);

      stepper2.moveTo(temp6);

      stepper1.run();
      stepper2.run();

      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;
   }

  // Moves piece to new y-coordinate
  if (coordinateFour >= 0){
    float temp7 = -pos7 * rotationFour;
    float temp8 = -pos8 * rotationFour;
    Serial.println(temp7);
    Serial.println(temp8);
    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(temp7);
      stepper2.moveTo(temp8);

      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }  
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;

  }

  magnetOff();

  while (Serial.available() > 0) {
    Serial.read();
  }

  delay(250);

  // Activates motors until limit switches are touched. 
  limitSwitch();
    while (Serial.available() > 0) {
    Serial.read();
  }
  
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
}

// Recieve user input for desired coordinates

int coordinateOneInput(){
  Serial.println("Coordinate One (Units right)?");      //Prompt User for input
  while (Serial.available() ==0 );           //Wait for user input
  coordinateOne = Serial.parseInt();
  return coordinateOne;
  
}

int coordinateTwoInput(){
  Serial.println("Coordinate Two (Units up)?");      //Prompt User for input
  while (Serial.available() ==0 );           //Wait for user input
  coordinateTwo = Serial.parseInt();
  return coordinateTwo;
  
}

int coordinateThreeInput(){
  Serial.println("Coordinate Three (Units right)?");      //Prompt User for input
  while (Serial.available() ==0 );           //Wait for user input
  coordinateThree = Serial.parseInt();
  return coordinateThree;
  
}

int coordinateFourInput(){
  Serial.println("Coordinate Four (Units up)?");      //Prompt User for input
  while (Serial.available() ==0 );           //Wait for user input
  coordinateFour = Serial.parseInt();
  return coordinateFour;
  
}

// Runs motors in specific direction until both limit switches are activated
void limitSwitch(){
      int flag3 = 0;
      int flag4 = 0; 
      limitswitch.loop();
      while (digitalRead(10) == HIGH){
          stepper1.move(200);

          stepper2.move(200);

          stepper1.run();
          stepper2.run();
          }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
      
    
      
    limitswitch2.loop();
    while (digitalRead(9) == HIGH){
            stepper1.move(200);

            stepper2.move(-200);

            stepper1.run();
            stepper2.run();


            }

     }


  
    
             
// Turns on electromagnet
void magnetOn(){
  digitalWrite(magnet, HIGH); 
  }

// Turns off electromagnet
void magnetOff(){
  digitalWrite(magnet, LOW);
  }
  
    
             

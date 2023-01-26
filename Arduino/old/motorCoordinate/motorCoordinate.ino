#include <AccelStepper.h>
#include <ezButton.h>


// Define pin connections
const int dirPin = 2;
const int stepPin = 3;
const int dirPin2 = 4;
const int stepPin2 = 5;

//define limit switches

ezButton limitswitch(7);
ezButton limitswitch2(8);


// Define two steppers and the pins they will use
AccelStepper stepper1(AccelStepper::DRIVER, stepPin, dirPin);
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);



int coordinateOne;    //Declare a float variable to hold your height
int coordinateTwo;
 
void setup() {
  Serial.begin(9600);      // Initialize Serial Port
  stepper1.setMaxSpeed(3000);
  stepper1.setAcceleration(2000);
  stepper2.setMaxSpeed(3000);
  stepper2.setAcceleration(2000);
  int pos1 = 200;
  int pos2 = 200;
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  limitswitch.setDebounceTime(5); // set debounce time to 50 milliseconds
  limitswitch2.setDebounceTime(5);
}
 
void loop() {
  coordinateOneInput();
  coordinateTwoInput();


  
  int flag1 = 1;
  int flag2 = 1;
  int pos1 = 200;
  int pos2 = 200;
  int pos3 = 200;
  int pos4 = 200;  
  

   int rotationOne = coordinateOne * 1;
   int rotationTwo = coordinateTwo * 1;

   if (coordinateOne >= 0){
    pos1 = pos1 * rotationOne;
    pos2 = -pos2 * rotationOne;    
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

  
   
  if (coordinateTwo >= 0){
    int temp3 = pos3 * rotationTwo;
    int temp4 = pos4 * rotationTwo;
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
  
  while (Serial.available() > 0) {
    Serial.read();
  }

  delay(1000);
  
  limitSwitch();
    while (Serial.available() > 0) {
    Serial.read();
  }
  
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
}

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


void limitSwitch(){
      int flag3 = 0;
      int flag4 = 0; 
      limitswitch.loop();
      while (digitalRead(7) == HIGH){
          stepper1.move(-200);

          stepper2.move(-200);

          stepper1.run();
          stepper2.run();
          }
      
    limitswitch2.loop();
    while (digitalRead(8) ==HIGH){
            stepper1.move(-200);

            stepper2.move(200);

            stepper1.run();
            stepper2.run();


            }

      }
    

  // Print well formatted output               

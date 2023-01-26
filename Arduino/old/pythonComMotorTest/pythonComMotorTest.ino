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



int rotations;    //Declare a float variable to hold your height
String stepDirection;
 
void setup() {
  Serial.begin(9600);      // Initialize Serial Port
  stepper1.setMaxSpeed(2000);
  stepper1.setAcceleration(2000);
  stepper2.setMaxSpeed(2000);
  stepper2.setAcceleration(2000);
  int pos1 = 200;
  int pos2 = 200;
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  limitswitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitswitch2.setDebounceTime(50);
}
 
void loop() {
  directionInput();
  rotationInput();

  Serial.print("You will move " + stepDirection + " " + rotations + " rotations");
  Serial.println();
  
  int flag1 = 1;
  int flag2 = 1;
//  int flag3 = 0;
//  int flag4 = 0;
  int pos1 = 200;
  int pos2 = 200;  
  
  if (stepDirection == "left"){
    int temp1 = -pos1 * rotations;
    int temp2 = -pos2 * rotations;
    pos1 =  temp1;
    pos2 =  temp2;
    Serial.println(temp1);
    Serial.println(pos2);
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
  }

  if (stepDirection == "right"){
    int temp3 = pos1 * rotations;
    int temp4 = pos2 * rotations;
    pos1 =  temp3;
    pos2 =  temp4;
    Serial.println(pos1);
    Serial.println(pos2);
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
  }

   if (stepDirection == "up"){
    int temp1 = -pos1 * rotations;
    int temp2 = pos1 * rotations;
    pos1 =  temp1;
    pos2 =  temp2;
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
   }

   if (stepDirection == "down"){
    pos1 = pos1 * rotations;
    pos2 = -1* pos2 * rotations;    
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
   }
  while (Serial.available() > 0) {
    Serial.read();
  }
  
  //limitSwitch();
    //while (Serial.available() > 0) {
    //Serial.read();
  //}
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
}

String directionInput(){
  Serial.println("What direction?");      //Prompt User for input
  while (Serial.available() ==0 );           //Wait for user input
  stepDirection = Serial.readString();
  return stepDirection;
  
}

int rotationInput(){
  Serial.println("How many rotations? ");      //Prompt User for input
  while (Serial.available() == 0);            //Wait for user input
  rotations = Serial.parseInt();              //Read user input and hold it in a variable
  return rotations;
  
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
//          limitswitch.loop();
//          if(limitswitch.isPressed()){
//            Serial.println("Limit 1 Pressed");
//            delay(1000);
//            limitswitch.getState();
//            flag3 = 1;
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

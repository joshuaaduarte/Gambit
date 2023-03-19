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

char lastM[5] = {'f','8','a','3','0'};
movement(lastM);
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
stepper1.setCurrentPosition(0);
stepper2.setCurrentPosition(0);
delay(10000);  


  

}


// Moves magnet to coordinate one

void movement(char lastM[5]){
    char rows[8] = {'1','2','3','4','5','6','7','8'};
    char columns[8] = {'a','b','c','d','e','f','g','h'};
    int rowStep[8] = {2075,1790,1505,1220, 935, 650, 365, 80};
    int columnStep[8] = {430,715,1000,1285,1570, 1855,2140,2425};
    int coordinateOneX = 0;
    int coordinateOneY = 0;
    int coordinateTwoX = 0;
    int coordinateTwoY = 0;

    int rotationOneX = 0;
    int rotationOneY = 0;
    int rotationTwoX = 0;
    int rotationTwoY = 0;

    int flag1 = 1;
    int flag2 = 1;

    const int magnet = 11;
    pinMode(magnet, OUTPUT); 
    // Break down given character array into coordinates
    for (int i=0; i<8; i++){
      if(lastM[0] == columns[i]){
        coordinateOneX = i;
        }
      if(lastM[2] == columns[i]){
        coordinateTwoX = i;
        }
    }
    for (int i=0; i<8; i++){
      if(lastM[1] == rows[i]){
        coordinateOneY = i;
        }
      if(lastM[3] == rows[i]){
        coordinateTwoY = i;
        }
    }
        
    rotationOneX = columnStep[coordinateOneX];
    rotationOneY = rowStep[coordinateOneY];

    rotationTwoX = columnStep[coordinateTwoX];
    rotationTwoY = rowStep[coordinateTwoY];
    digitalWrite(magnet, HIGH); 
    // Move to initial X coordinate
    while (flag1 == 1 && flag2 == 1){
      
      stepper1.moveTo(-rotationOneX);

      stepper2.moveTo(rotationOneX);

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

    //Move to initial Y coordinate
    while (flag1 == 1 && flag2 == 1){
      
      stepper1.moveTo(-rotationOneY);

      stepper2.moveTo(-rotationOneY);

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

    digitalWrite(magnet, LOW); 

}

    
    
    






// Runs motors in specific direction until both limit switches are activated
//      void homing(){
//        
//        limitswitch.loop();
//        while (digitalRead(10) == HIGH){
//            stepper1.move(200);
//            stepper2.move(200);
//            stepper1.run();
//            stepper2.run();
//        }
//        
//        stepper1.setCurrentPosition(0);
//        stepper2.setCurrentPosition(0);
//                          
//        limitswitch2.loop();
//        while (digitalRead(9) == HIGH){
//        stepper1.move(200);
//        stepper2.move(-200);
//        stepper1.run();
//        stepper2.run();
//        }
//     }
//
//
//  
//    
//             
//// Turns on electromagnet
//  void magnetOn(){
//    digitalWrite(magnet, HIGH); 
//    }
//
//// Turns off electromagnet
//  void magnetOff(){
//    digitalWrite(magnet, LOW);
//    }
//    
    
             

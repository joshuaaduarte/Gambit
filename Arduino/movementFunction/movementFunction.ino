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

char lastM[5] = {'b','3','b','7','0'};
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
    int rowStep[8] = {2075,1805,1525,1220, 945, 650, 365, 70};
    int columnStep[8] = {430,715,1020,1300,1590, 1885,2175,2450};
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

    // Determine what type of movement it is
    // Conquer
    // Castle
    // Horizontal
    if (coordinateOneY == coordinateTwoY){
      // Move to coordinate One
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
      // Turn on Magnet
      digitalWrite(magnet, HIGH);
      rotationTwoX = abs(columnStep[coordinateTwoX]-columnStep[coordinateOneX]);
      rotationTwoY = abs(rowStep[coordinateTwoY]-rowStep[coordinateOneY]);
      
      if (coordinateTwoX > coordinateOneX){
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationTwoX);
        stepper2.moveTo(rotationTwoX);
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
      else{
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(rotationTwoX);
        stepper2.moveTo(-rotationTwoX);
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
    }

    // vertical
    if (coordinateOneX == coordinateTwoX){
      // Move to coordinate One
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
      // Turn on Magnet
      digitalWrite(magnet, HIGH);
      rotationTwoX = abs(columnStep[coordinateTwoX]-columnStep[coordinateOneX]);
      rotationTwoY = abs(rowStep[coordinateTwoY]-rowStep[coordinateOneY]);
      
      if (coordinateTwoY > coordinateOneY){
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(rotationTwoY+55);
        stepper2.moveTo(rotationTwoY+55);
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
      else{
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationTwoY);
        stepper2.moveTo(-rotationTwoY);
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
    }


    // diagonal

    // knight
//    int rowChange = abs(coordinateOneY - coordinateTwoY);
//    int colChange = abs(coordinateOneX - coordinateTwoX);
//
//    if (rowChange == 2 && colChange == 1) or (rowChange == 1 && colChange == 2){
//      // Move to coordinate One
//      // Move to initial X coordinate
//      while (flag1 == 1 && flag2 == 1){
//        stepper1.moveTo(-rotationOneX);
//        stepper2.moveTo(rotationOneX);
//        stepper1.run();
//        stepper2.run();
//        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
//          flag1 = 0;
//          flag2 = 0;
//        }
//      }
//      stepper1.setCurrentPosition(0);
//      stepper2.setCurrentPosition(0);
//      flag1 = 1;
//      flag2 = 1;
//      //Move to initial Y coordinate
//      while (flag1 == 1 && flag2 == 1){
//        stepper1.moveTo(-rotationOneY);
//        stepper2.moveTo(-rotationOneY);
//        stepper1.run();
//        stepper2.run();
//        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
//          flag1 = 0;
//          flag2 = 0;
//        }
//      }
//      stepper1.setCurrentPosition(0);
//      stepper2.setCurrentPosition(0);
//      flag1 = 1;
//      flag2 = 1;
//      // Turn on Magnet
//      digitalWrite(magnet, HIGH);
//            
//      // if knight is moving to right
//        //move knight over to line
//        while (flag1 == 1 && flag2 == 1){
//        stepper1.moveTo(-100);
//        stepper2.moveTo(100);
//        stepper1.run();
//        stepper2.run();
//        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
//          flag1 = 0;
//          flag2 = 0;
//        }
//        }
//        stepper1.setCurrentPosition(0);
//        stepper2.setCurrentPosition(0);
//        flag1 = 1;
//        flag2 = 1;  
//               
//        }
//      // if knight is moving to left 
    
//    }
    
    //digitalWrite(magnet, HIGH); 
    
//    // Move to initial X coordinate
//    while (flag1 == 1 && flag2 == 1){
//      
//      stepper1.moveTo(-rotationOneX);
//
//      stepper2.moveTo(rotationOneX);
//
//      stepper1.run();
//      stepper2.run();
//
//      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
//        flag1 = 0;
//        flag2 = 0;
//      }
//    }
//    stepper1.setCurrentPosition(0);
//    stepper2.setCurrentPosition(0);
//    flag1 = 1;
//    flag2 = 1;
//
//    //Move to initial Y coordinate
//    while (flag1 == 1 && flag2 == 1){
//      
//      stepper1.moveTo(-rotationOneY);
//
//      stepper2.moveTo(-rotationOneY);
//
//      stepper1.run();
//      stepper2.run();
//
//      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
//        flag1 = 0;
//        flag2 = 0;
//      }
//    }
//    stepper1.setCurrentPosition(0);
//    stepper2.setCurrentPosition(0);
//    flag1 = 1;
//    flag2 = 1;

    digitalWrite(magnet, LOW); 
    delay(1000);

}

    
    
    







             

//  // Stepper Motor X
//  const int stepPin = 3; //X.STEP
//  const int dirPin = 6; // X.DIR
// 
// void setup() {
// // Sets the two pins as Outputs
// pinMode(stepPin,OUTPUT); 
// pinMode(dirPin,OUTPUT);
// }
// void loop() {
// digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
// // Makes 200 pulses for making one full cycle rotation
// for(int x = 0; x < 200; x++) {
// digitalWrite(stepPin,HIGH); 
// delayMicroseconds(500); 
// digitalWrite(stepPin,LOW); 
// delayMicroseconds(500); 
// }
// delay(1000); // One second delay
// 
// digitalWrite(dirPin,LOW); //Changes the rotations direction
// // Makes 400 pulses for making two full cycle rotation
// for(int x = 0; x < 400; x++) {
// digitalWrite(stepPin,HIGH);
// delayMicroseconds(500);
// digitalWrite(stepPin,LOW);
// delayMicroseconds(500);
// }
// delay(1000);
// }

/*
    Controlling multiple steppers with the AccelStepper and MultiStepper library

     by Dejan, https://howtomechatronics.com
*/

#include <AccelStepper.h>
#include <MultiStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(1, 2, 5); // (Typeof driver: with 2 pins, STEP, DIR)
AccelStepper stepper2(1, 3, 6);
//AccelStepper stepper3(1, 4, 7);

MultiStepper steppersControl;  // Create instance of MultiStepper

long gotoposition[3]; // An array to store the target positions for each stepper motor

void setup() {

  stepper1.setMaxSpeed(12000); // Set maximum speed value for the stepper
  stepper2.setMaxSpeed(12000);
 // stepper3.setMaxSpeed(1000);

  // Adding the 3 steppers to the steppersControl instance for multi stepper control
  steppersControl.addStepper(stepper1);
  steppersControl.addStepper(stepper2);
  //steppersControl.addStepper(stepper3);
}

void loop() {
  // Store the target positions in the "gotopostion" array
  gotoposition[0] = 20000;  // 800 steps - full rotation with quater-step resolution
  gotoposition[1] = 36000;
  //gotoposition[2] = 3200;

  steppersControl.moveTo(gotoposition); // Calculates the required speed for all motors
  steppersControl.runSpeedToPosition(); // Blocks until all steppers are in position

  delay(1000);

  gotoposition[0] = 0;
  gotoposition[1] = 0;
  //gotoposition[2] = 0;

  steppersControl.moveTo(gotoposition);
  steppersControl.runSpeedToPosition();

  delay(1000);
}

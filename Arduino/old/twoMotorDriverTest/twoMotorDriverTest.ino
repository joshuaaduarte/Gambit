// Include the AccelStepper Library
#include <AccelStepper.h>

// Define pin connections
const int dirPin = 2;
const int stepPin = 3;
const int dirPin2 = 4;
const int stepPin2 = 5;


#include <AccelStepper.h>

// Define two steppers and the pins they will use
AccelStepper stepper1(AccelStepper::DRIVER, stepPin, dirPin);
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);

int pos1 = 2000;
int pos2 = 2000;

void setup()
{ 
  stepper1.setMaxSpeed(2000);
  stepper1.setAcceleration(2000);
  stepper2.setMaxSpeed(2000);
  stepper2.setAcceleration(2000);
}

void loop()
{
  if (stepper1.distanceToGo() == 0)
  {
     pos1 = -pos1;
    stepper1.moveTo(pos1);
  }
  if (stepper2.distanceToGo() == 0)
  {
    pos2 = -pos2;
    stepper2.moveTo(pos2);
  }
  stepper1.run();
  stepper2.run();
}

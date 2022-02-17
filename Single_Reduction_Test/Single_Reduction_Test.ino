
#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>

// Define a stepper and the pins it will use
AccelStepperWithDistance stepper(1, 5, 4); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(300);
  stepper.setStepsPerRotation(200);
  stepper.setAnglePerRotation(68.9362);
}

void loop()
{
  stepper.runToNewAngle(360);
  delay(1000);
  stepper.runToNewAngle(0);
  delay(1000);
}

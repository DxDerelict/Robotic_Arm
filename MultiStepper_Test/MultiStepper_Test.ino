// MultiStepper.pde
// -*- mode: C++ -*-
// Use MultiStepper class to manage multiple steppers and make them all move to
// the same position at the same time for linear 2d (or 3d) motion.

#include <AccelStepper.h>
#include <MultiStepper.h>

/////// Code Block for Serial Comms ///////////
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;
///////////////////////////////////////////////

#define motorInterfaceType 1
#define MAXSPEED 200

AccelStepper Base(motorInterfaceType, 3, 2);
AccelStepper J1 (motorInterfaceType, 5, 4);
AccelStepper J2 (motorInterfaceType, 7, 6);

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;
const uint8_t numOfSteppers = 5;
long positions[numOfSteppers] = {100, 200, 300, 400, 500};
int stepperVal[numOfSteppers];

void setup() {
  Serial.begin(115200);

  // Configure each stepper
  Base.setMaxSpeed(MAXSPEED);
  J1.setMaxSpeed(MAXSPEED);
  J2.setMaxSpeed(MAXSPEED);

  // Then give them to MultiStepper to manage
  steppers.addStepper(Base);
  steppers.addStepper(J1);
  steppers.addStepper(J2);


  long positions[3] = {800, 500, -300}; // Array of desired stepper positions
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);
}

void loop() {
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);  // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseIntegers(tempChars, numChars, stepperVal, numOfSteppers);
    showParsedInts();
    runStepperSim();
    newData = false;
  }

}

void runStepperSim() {
  Serial.print("Simultaneously Running to position... ");

  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position

  delay(1000);
//
//  for (int i = 0; i < numOfSteppers; i++) {
//    stepper[i].moveTo(positions[i]);
//    stepper[i].setSpeed(motorSpeed);
//  }
//
//  while ( (stepper[0].distanceToGo() != 0) || (stepper[1].distanceToGo() != 0) || (stepper[2].distanceToGo() != 0)) {
//    for (int i = 0; i < numOfSteppers; i++) {
//      stepper[i].runSpeedToPosition();
//    }
//  }
//
//  for (int i = 0; i < numOfSteppers; i++) {
//    stepper[i].setCurrentPosition(0);
//  }
  Serial.println("done");
}

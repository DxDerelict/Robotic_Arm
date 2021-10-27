#include <AccelStepper.h>
#define motorInterfaceType 1

//AccelStepper stepper1 = AccelStepper(motorInterfaceType, 3, 2); //  (motorInterfaceType, stepPin, dirPin)
//AccelStepper stepper2 = AccelStepper(motorInterfaceType, 5, 4);
//AccelStepper stepper3 = AccelStepper(motorInterfaceType, 7, 6);

const unsigned int numOfSteppers = 3;
AccelStepper stepper[numOfSteppers] = {
  AccelStepper(motorInterfaceType, 3, 2),
  AccelStepper(motorInterfaceType, 5, 4),
  AccelStepper(motorInterfaceType, 7, 6)
};


int motorMaxSpeed = 1000;
int motorSpeed = 600;

long positions[3] = {1000, 1000, 1000};
int stepperVal[3];

/////// Code Block for Serial Comms ///////////
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;
///////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println("This demo expects 3 sets of integers");
  Serial.println("Enter data in this style <X, Y, Z> ");
  Serial.println();
  
  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].setMaxSpeed(motorMaxSpeed);
  }
  delay(500);
  motorTestRun();
}

void loop() {
  // Start taking in Serial coordinates
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);  // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseIntegers();
    showParsedInts();
    runStepperSim();
    newData = false;
  }

}

//void runStepperSeq() {
//  for (int i = 0; i < 3; i++) {
//    positions[i] = stepperVal[i];
//  }
//  Serial.print("\n   Running Steppers to position... Sequentially");
//
//  stepper1.moveTo(positions[0]);
//  stepper1.setSpeed(motorSpeed);
//
//  stepper2.moveTo(positions[1]);
//  stepper2.setSpeed(motorSpeed);
//
//  while (stepper1.currentPosition() != stepper1.targetPosition()) {
//    stepper1.runSpeedToPosition();
//  }
//  while (stepper2.currentPosition() != stepper2.targetPosition()) {
//    stepper2.runSpeedToPosition();
//  }
//  Serial.println("done");
//
//}

void runStepperSim() {
  Serial.print("Simultaneously Running to position... ");

  for (int i = 0; i < numOfSteppers; i++) {
    positions[i] = stepperVal[i];
  }

  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].moveTo(positions[i]);
    stepper[i].setSpeed(motorSpeed);
  }

  while ( (stepper[0].distanceToGo() != 0) || (stepper[1].distanceToGo() != 0) || (stepper[2].distanceToGo() != 0)) {
    for (int i = 0; i < numOfSteppers; i++) {
      stepper[i].runSpeedToPosition();
    }
  }

  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].setCurrentPosition(0);
  }
  Serial.println("done");
}

void motorTestRun() {
  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].moveTo(positions[i]);
    stepper[i].setSpeed(motorSpeed);
  }

  for (int i = 0; i < numOfSteppers; i++) {
    while (stepper[i].distanceToGo() != 0) {
      stepper[i].runSpeedToPosition();
    }
  }
  Serial.println("Initial Test Run Complete");
}

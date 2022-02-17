#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>
#define motorInterfaceType 1

#define BASE_PUL 2
#define BASE_STEP 3
#define J1_PUL 4
#define J1_STEP 5
#define J2_PUL 6
#define J2_STEP 7


//  (motorInterfaceType, stepPin, dirPin)
//AccelStepper stepper1 = AccelStepper(motorInterfaceType, 3, 2);
//AccelStepper stepper2 = AccelStepper(motorInterfaceType, 5, 4);
//AccelStepper stepper3 = AccelStepper(motorInterfaceType, 7, 6);

/////// Code Block for Serial Comms ///////////
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;
///////////////////////////////////////////////

const uint8_t numOfSteppers = 5;
AccelStepperWithDistance stepper[numOfSteppers] = {
  AccelStepperWithDistance(motorInterfaceType, BASE_STEP, BASE_PUL),
  AccelStepperWithDistance(motorInterfaceType, J1_STEP, J1_PUL),
  AccelStepperWithDistance(motorInterfaceType, J2_STEP, J2_PUL)
};

int motorMaxSpeed = 500;
int motorSpeed = 300;

long positions[numOfSteppers] = {400, 400, 300, 400, 500};
int stepperVal[numOfSteppers];


void setup() {
  Serial.begin(115200);
  Serial.println("This demo expects 3 sets of integers");
  Serial.println("Enter data in this style <X, Y, Z> ");
  Serial.println();

  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].setMaxSpeed(motorMaxSpeed);
    stepper[i].setAcceleration(motorMaxSpeed);
  }

  stepper[0].setMaxSpeed(300);
  stepper[0].setAcceleration(300);
  stepper[0].setStepsPerRotation(200);
  stepper[0].setAnglePerRotation(68.9362);
  delay(500);
  motorTestRun();
}

void loop() {
  // Start taking in Serial coordinates
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);  // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseIntegers(tempChars, numChars, stepperVal, numOfSteppers);
    for (int i = 0; i < 3; i++) {
      positions[i] = stepperVal[i];
    }
    showParsedInts();
    runStepperSim();
    newData = false;
  }

}

void runStepperSim() {
  Serial.print("Simultaneously Running to position... ");

  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].moveToAngle(stepperVal[i]);
    //    stepper[i].setSpeed(motorSpeed);
  }

  while ( (stepper[0].distanceToGo() != 0) || (stepper[1].distanceToGo() != 0) || (stepper[2].distanceToGo() != 0)) {
    for (int i = 0; i < numOfSteppers; i++) {
      stepper[i].run();
    }
  }
  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].setCurrentPosition(0);
  }

  Serial.print("done");
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
  for (int i = 0; i < numOfSteppers; i++) {
    stepper[i].setCurrentPosition(0);
  }
  Serial.println("Initial Test Run Complete");
}

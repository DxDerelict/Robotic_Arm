void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void parseIntegers() {      // split the data into its parts
  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  stepperVal[0] = atoi(strtokIndx);         // convert this part to an integer

  strtokIndx = strtok(NULL, ",");         // this continues where the previous call left off
  stepperVal[1] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  stepperVal[2] = atoi(strtokIndx);

  for (int i = 0; i < 3; i++) {
    positions[i] = stepperVal[i];
  }

}
void showParsedInts() {
  //  Serial.print("s1: ");
  //  Serial.print(stepperVal[0]);
  //  Serial.print("  s2: ");
  //  Serial.print(stepperVal[1]);
  //  Serial.print("  s3: ");
  //  Serial.println(stepperVal[2]);

  char buff[32];
  sprintf(buff, " s1:%d s2:%d s3:%d ", stepperVal[0] , stepperVal[1], stepperVal[2]);
  Serial.println(buff);
}

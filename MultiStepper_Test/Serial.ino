
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

void parseIntegers(char msg[], int msgSize, int Val[], int ValSize) {      // split the data into its parts
  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(msg, ",");     // get the first part - the string
  Val[0] = atoi(strtokIndx);         // convert this part to an integer

  strtokIndx = strtok(NULL, ",");         // this continues where the previous call left off
  Val[1] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  Val[2] = atoi(strtokIndx);

}
void showParsedInts() {
  char buff[32];
  sprintf(buff, " s1:%d s2:%d s3:%d ", stepperVal[0] , stepperVal[1], stepperVal[2]);
  Serial.println(buff);
}

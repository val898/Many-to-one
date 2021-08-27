//------------------Array Utilities------------------


void resetCharArray(char theArray[], int theArraySize) {
#ifdef VERBOSE
  Serial.println("IN :: clearCharArray");
#endif
  DEBUG_PRINTLN("clearCharArray");
  sprintf(theArray, "%2.15f", 0.0f );// fill it with zeros
  theArray[theArraySize - 1] = '\0'; // null terminate it
}


void clearCharArray(char theArray[], int theArraySize) {
#ifdef VERBOSE
  Serial.println("IN :: clearCharArray");
#endif
  DEBUG_PRINTLN("clearCharArray");
  for (int i = 0; i < theArraySize; i++ ) {
    theArray[i] = '\0';
  }
  // this feels redundant ....
  theArray[theArraySize - 1] = '\0'; // null terminate it
}


void printCharArrayElements(char theArray[], int theArraySize) {
#ifdef VERBOSE
  Serial.println("IN :: printCharArrayElements");
#endif
#ifdef DEBUG
  for (int i = 0; i < theArraySize; i ++ ) {
    Serial.print("element[");
    Serial.print(i);
    Serial.print("]\t");
    Serial.println(theArray[i]);
  }
#endif
}


void printBuffer(char theArray[], int messageSize) {
  Serial.println("print buffer --> ");
  for (int i = 0; i < messageSize; i++) {
    if ( '\0' == theArray[i]) break;
    Serial.print(theArray[i]);
  }
  Serial.println("\n<-- end buffer");
}


float convertToFloat( char theArray[] ) {
#ifdef VERBOSE
  Serial.println("IN :: convertToFloat");
#endif
  // not incoming array MUST be null terminated
  // convert char array to float
  return atof(theArray);
}


int convertToInt( char theArray[] ) {
#ifdef VERBOSE
  Serial.println("IN :: convertToInt");
#endif
  // not incoming array MUST be null terminated
  // convert char array to float
  return atoi(theArray);
}

// https://overiq.com/c-programming-101/array-of-pointers-to-strings-in-c/

// also
// see file :: C-files_Testing/simpleParse/messageParse.cpp
// and
// C-files_Testing/simpleParse/split.cpp


// check for start end
// number of tokens will be assoicated with topic -- structure will be known and global ?
int  checkMessage(char inMessage[], int messageLength) {
  //Serial.println("IN checkMessage->");

  int status = 6;
  Serial.print("IN checkMessage-> ");
  Serial.println(inMessage);

  if (inMessage[0] != START_MARKER) {
    Serial.println("\tfailNoStart");
    status = 4;
  } else if (inMessage[messageLength - 1] != END_MARKER) {
    Serial.println("\tfailNoEnd");
    status = 5;
  } else {
    Serial.println("\treceived a complete message");
    // get substring without first / last
    for (int i = 1; i < messageLength - 1; i++ ) {
      cleanMessage[i - 1] = inMessage[i];
    }
    cleanMessage[messageLength - 2] = '\0';
    needParse = true;
    status = 1;
  }

  return status;
}

// recall this is teh array of pointers to hold tokens
// char *bufferSplit[8];

// int parseMessage(char testString[], int numberOfTokens) {      // split the data into its parts
int parseMessage() {

  // use cleanMessage and strtok to get array of pointers to char[]s

  // messages getting here will have 1 or more tokens
  // first token is ALWAYS id -- stored to idToken
  // remaining tokens are data --
  // need to parse and then check id

  static int parseStatus = 5;
  Serial.print( "IN parseData->\t");
  Serial.println( cleanMessage ); // won't work cause not how arduino handles arrays

  char * strtokIndx; // this is used by strtok() as an index
  //strcpy(tempChars, testString);
  //strtokIndx = strtok(tempChars, ",");     // get the first part - the string

  // get the first token = message id
  strtokIndx = strtok(cleanMessage, ",");
  bufferSplit[0] = strtokIndx; // store the id in index 0

    // got message and its my id get the data
    Serial.println( "\tgot message with my id or broadcast");
    parseStatus = 3;
    int counter = 0;
    while (strtokIndx != NULL) { // cause strtok return NULL pointer when none left
      // for (int i = 1; i < numberOfTokens; i ++){
      Serial.print( "IN while  -->\t");
      Serial.println(strtokIndx);
      bufferSplit[counter++] = strtokIndx;
      strtokIndx = strtok(NULL, ",");
      //  counter++;
    }  // end while
    Serial.print("in parse message --> bufferSplit[1] :: \t");
    Serial.println(bufferSplit[1]);
    parseStatus = 1; // parsed

  return parseStatus;
}

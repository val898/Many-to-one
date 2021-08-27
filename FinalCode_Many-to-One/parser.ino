//------------------Parser------------------


// https://overiq.com/c-programming-101/array-of-pointers-to-strings-in-c/

// also
// see file :: C-files_Testing/simpleParse/messageParse.cpp
// and
// C-files_Testing/simpleParse/split.cpp


// check for start end
// number of tokens will be assoicated with topic -- structure will be known and global ?
int  checkMessage(char inMessage[], int messageLength) {

  int status = 6;
  DEBUG_PRINT("IN checkMessage-> ");
  DEBUG_PRINTLN(inMessage);

  if (inMessage[0] != START_MARKER) {
    DEBUG_PRINTLN("\tfailNoStart");
    status = 4;
  } else if (inMessage[messageLength - 1] != END_MARKER) {
    DEBUG_PRINTLN("\tfailNoEnd");
    status = 5;
  } else {
    DEBUG_PRINTLN("\treceived a complete message");
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

// recall this is the array of pointers to hold tokens

int parseMessage() {

  // use cleanMessage and strtok to get array of pointers to char[]s

  // messages getting here will have 1 or more tokens
  // first token is ALWAYS id -- stored to idToken
  // remaining tokens are data --
  // need to parse and then check id

  static int parseStatus = 5;
  DEBUG_PRINT( "IN parseData->\t");
  DEBUG_PRINTLN( cleanMessage ); // won't work cause not how arduino handles arrays

  char * strtokIndx; // this is used by strtok() as an index

  // get the first token = message id
  strtokIndx = strtok(cleanMessage, ",");
  bufferSplit[0] = strtokIndx; // store the id in index 0

    // got message and its my id get the data
    DEBUG_PRINTLN( "\tgot message with my id or broadcast");
    parseStatus = 3;
    int counter = 0;
    while (strtokIndx != NULL) { // cause strtok return NULL pointer when none left
      DEBUG_PRINT( "IN while  -->\t");
      DEBUG_PRINTLN(strtokIndx);
      bufferSplit[counter++] = strtokIndx;
      strtokIndx = strtok(NULL, ",");
    }  // end while
    DEBUG_PRINT("in parse message --> bufferSplit[1] :: \t");
    DEBUG_PRINTLN(bufferSplit[1]);
    parseStatus = 1; // parsed

  return parseStatus;
}

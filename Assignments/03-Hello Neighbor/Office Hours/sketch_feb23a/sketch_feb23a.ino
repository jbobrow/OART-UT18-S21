void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  bool doesBlinkHaveAnyNeighbors = false;

  // look at all of our faces
  FOREACH_FACE(f) {
    
    if(isValueReceivedOnFaceExpired(f)) { // 200 ms have passed since I've seen a message from a Blink
      // there is not another Blink present
    }
    else {
      // another Blink is here
      doesBlinkHaveAnyNeighbors = true;
    }
    
  }

  if(doesBlinkHaveAnyNeighbors) {
    // display a signal to say we have neighbors
    setColor(BLUE); // I have neighbors
  }
  else {
    // display that we are alone
    setColor(RED); // I am lonely
  }
  
}

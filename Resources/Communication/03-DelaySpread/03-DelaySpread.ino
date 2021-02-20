/*
   Let's make Blinks talk to each other
   Share our value
*/

Timer waitTimer;
#define WAIT_DURATION 1000

bool isPulsing = false;  // 1 = TRUE and 0 = FALSE

void setup() {
  // put your setup code here, to run once:
  waitTimer.never();
}

void loop() {
  // put your main code here, to run repeatedly:

  // react to user input
  if (buttonPressed()) {
    isPulsing = !isPulsing;
  }

  // Listen to my neighbors
  FOREACH_FACE(f) {
    // loop through all of them
    if (!isValueReceivedOnFaceExpired(f)) {
      // if my neighbor is present, get their value

      byte neighborVal = getLastValueReceivedOnFace(f); // listen

      // set my value, to be the same as my neighbor
      if (neighborVal == 1 && waitTimer.isExpired()) {
        waitTimer.set(WAIT_DURATION);
      }
    }
  }

  // wait a period of time before reacting
  if(waitTimer.isExpired()) {
    isPulsing = true;
    waitTimer.never();
  }

  // Tell everyone if we are pulsing
  if (isPulsing) {
    setValueSentOnAllFaces(1);
  }
  else {
    setValueSentOnAllFaces(0);
  }


  // display
  if(isPulsing) {
    setColor( dim(WHITE, sin8_C(millis()/2)) );
  }
  else {
    setColor( WHITE );
  }

  
}

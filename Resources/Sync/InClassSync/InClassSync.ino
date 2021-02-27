/*



*/

// internal timer
#define PERIOD_DURATION 2000  // 2000 ms or 2 seconds
Timer syncTimer;

// let's keep track of our neighbors
byte neighborState[6];

byte myState = 0; // flips between 1 and 0,


// THIS WILL HAPPEN ONCE
void setup() {

}


// THIS HAPPENS ~30 TIMES PER SECOND
void loop() {

  if (buttonPressed()) {
    // reset our internal timer
    syncTimer.set(PERIOD_DURATION);
  }

  // broadcast my state
  setValueSentOnAllFaces(myState);

  // create a function to keep our timer in sync with our neighbors timers
  syncLoop();

  // display the progress of our timer
  if (syncTimer.getRemaining() < 400 ) {
    setColor(WHITE);
  }
  else {
    setColor(OFF);
  }

}




void syncLoop() {

  // let's look for a neighbor that changed
  bool didNeighborChange = false;

  // look at all neighbors, and compare to the last time we saw them
  FOREACH_FACE(face) {
    if ( !isValueReceivedOnFaceExpired(face) ) {

      // get our neighbor value
      byte neighborVal = getLastValueReceivedOnFace(face);

      // check if changed
      if ( neighborState[face] != neighborVal ) {
        // value has changed
        didNeighborChange = true;
      }

      // then store the value
      neighborState[face] = neighborVal;
    }
  }

  // reset our time to full time after we've reach zero
  if (syncTimer.isExpired() || (didNeighborChange && syncTimer.getRemaining() < PERIOD_DURATION * 0.9) ) {  // the conditions under which we reset
    
    // reset our timer
    syncTimer.set(PERIOD_DURATION);

    // update our state
    if (myState == 0) {
      myState = 1;
    }
    else {
      myState = 0;
    }
  }


}

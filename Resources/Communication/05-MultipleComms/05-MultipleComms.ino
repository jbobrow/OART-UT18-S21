/*
 * Sending multiple pieces of information in the same message
 * 1) Have Blinks share a timer to stay in sync
 * 2) Have Blinks share their identity with neighbors
 * Both of these pieces of information are being shared at the same time
 * 
 * We learned:
 * Binary representation
 * Bitshifting '>>' and '<<'
 * Bitwise '&' operator 
 */

// internal timer
#define PERIOD_DURATION 2000  // 2000 ms or 2 seconds
Timer syncTimer;

// let's keep track of our neighbors
byte neighborState[6];

byte myState = 0; // flips between 1 and 0,

enum Identity{
  SPY,
  THIEF,
  INNOCENT,
  GUILTY
};

Color idColors[4] = {GREEN, RED, YELLOW, BLUE};

byte myIdentity = SPY;

// A Byte's digits represent the following: 
// 128 64 32 16 8 4 2 1
// setValueSentOnAllFaces() only uses the non X locations (i.e. 0-63) 
// 1 = XX000001
// 2 = XX000010
// 3 = XX000011
// 7 = XX000111
//15 = XX001111
 
// THIS WILL HAPPEN ONCE
void setup() {
  randomize();
  myIdentity = random(3);
}


// THIS HAPPENS ~30 TIMES PER SECOND
void loop() {

  if (buttonPressed()) {
    // reset our internal timer
    syncTimer.set(PERIOD_DURATION);
  }

  // broadcast my state
  // myIdentity uses [ ][ ][ ][X][X][ ]
  // myState uses    [ ][ ][ ][ ][ ][X]
  byte sendData = (myIdentity << 1) + myState;

  // send a bundle of information (myIdentity) and (myState)
  setValueSentOnAllFaces(sendData);

  // create a function to keep our timer in sync with our neighbors timers
  syncLoop();

  // display the progress of our timer
  if (syncTimer.getRemaining() < 400 ) {
    setColor(WHITE);
  }
  else {
    setColor(OFF);
  }

  // display the identity of our neighbor on face 0
  if(!isValueReceivedOnFaceExpired(0)) {
    // get the identity of my neighbor from the value received on the face
    byte neighborId = getNeighborIdentity(getLastValueReceivedOnFace(0));
    setColorOnFace(idColors[neighborId], 0);
  }

  // ADDED After Class
  // when my button is down, reveal my identity
  // a nice way to confirm that my neighbor is receiving my real identity
  if(buttonDown()) {
    // show my identity on my Blink
    setColor(idColors[myIdentity]); 
  }
}


byte getNeighborState(byte data) {

    // myState uses    [ ][ ][ ][ ][ ][X]
    // & 1 means we only keep the following [ ][ ][ ][ ][ ][X]
    // & 3 means we only keep the following [ ][ ][ ][ ][X][X]
    // & 2 means we only keep the following [ ][ ][ ][ ][X][ ]
    // &  
    byte neighborState = data & 1;
    return neighborState;
}

byte getNeighborIdentity(byte data) {

  // myIdentity uses [ ][ ][ ][X][X][ ]
  byte neighborIdentity = (data >> 1) & 3;
  
  return neighborIdentity;
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
      if ( neighborState[face] != getNeighborState(neighborVal) ) {
        // value has changed
        didNeighborChange = true;
      }

      // then store the value
      neighborState[face] = getNeighborState(neighborVal);
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

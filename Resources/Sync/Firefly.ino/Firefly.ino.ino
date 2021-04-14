/*
 * Examples of Blinks in Sync
 * 
 */
Timer syncTimer;
#define PERIOD_DURATION 2000
byte neighborState[6];
byte syncVal = 0;

#define ABSENT 64

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  if(buttonPressed()) {
    syncTimer.set(PERIOD_DURATION);
  }

  syncLoop();

  // brighter when closer to expired
  byte bri = 255 - map(syncTimer.getRemaining(), 0, PERIOD_DURATION, 0, 255);
  setColor(dim(WHITE, bri));

  setValueSentOnAllFaces(syncVal);
}

/*
   Firefly motivation
*/
void syncLoop() {

  bool didNeighborChange = false;

  // look at our neighbors to determine if one of them passed go (changed value)
  // note: absent neighbors changing to not absent don't count
  FOREACH_FACE(f) {
    if (isValueReceivedOnFaceExpired(f)) {
      neighborState[f] = ABSENT; // this is an absent neighbor
    }
    else {
      byte neighborVal = getLastValueReceivedOnFace(f);
      if (neighborState[f] != ABSENT) {  // wasn't absent
        if (neighborVal != neighborState[f]) { // passed go (changed value)
          didNeighborChange = true;
        }
      }

      neighborState[f] = neighborVal;  // update our record of state now that we've check it
    }
  }

  // if our neighbor passed go and we haven't done so within the buffer period, catch up and pass go as well
  // if we are due to pass go, i.e. timer expired, do so
  if ( (didNeighborChange && syncTimer.getRemaining() < PERIOD_DURATION * 0.9 )
       || syncTimer.isExpired()
     ) {

    syncTimer.set(PERIOD_DURATION); // aim to pass go in the defined duration
    
    if(syncVal == 0 ) { // change our value everytime we pass go
      syncVal = 1;
    }
    else {
      syncVal = 0;
    }
  }
}

/*
 * 
 */
 
#define STEP_DURATION 1000
Timer stepTimer;

enum States {
  WATER,
  BEACH,
  LAND
};

byte myState = WATER;

void setup() {
  // put your setup code here, to run once:
  randomize();
  init();
  stepTimer.set(STEP_DURATION);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (stepTimer.isExpired()) {
    stepTimer.set(STEP_DURATION);

    // update based on our state
    switch (myState) {
      case WATER: waterLoop(); break;
      case BEACH: beachLoop(); break;
      case LAND: landLoop(); break;
    }
  }

  // display our state
  switch (myState) {
    case WATER: setColor(CYAN); break;
    case BEACH: setColor(YELLOW); break;
    case LAND: setColor(GREEN); break;
  }

  // share my state
  setValueSentOnAllFaces(myState);

}

void init() {
  // create 50% water
  // 25% beach
  // 25% land
  if (random(100) < 50) {
    myState = WATER;
  }
  else {
    if (random(100) < 50) {
      myState = BEACH;
    }
    else {
      myState = LAND;
    }
  }
}

byte countNeigborsOfType(byte type) {

  byte count = 0;

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte neighborVal = getLastValueReceivedOnFace(f);
      if (neighborVal == type) {
        count++;
      }
    }
  }

  return count;
}

void waterLoop() {
  // if more than X neighbors are BEACH, then become a BEACH
  if (countNeigborsOfType(BEACH) > 3) {
    myState = BEACH;
  }
}

void beachLoop() {
  // If most neighbors are water, become water
  if (countNeigborsOfType(WATER) > 4 ) {
    myState = WATER;
  }

  // if NO neighbors are water, become land
  if (countNeigborsOfType(WATER) == 0) {
    myState = LAND;
  }
}

void landLoop() {

  //if ANY neighbors are water, become beach
  if (countNeigborsOfType(WATER) >= 1) {
    myState = BEACH;
  }
}

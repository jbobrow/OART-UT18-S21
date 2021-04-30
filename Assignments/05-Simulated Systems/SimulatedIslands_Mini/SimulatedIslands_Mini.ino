/*

*/


#define STEP_DURATION 1000
Timer stepTimer;

enum States {
  WATER,
  BEACH,
  LAND
};

byte myStates[6] = {WATER, WATER, WATER, WATER, WATER, WATER};

void setup() {
  // put your setup code here, to run once:
  randomize();
  init();
  stepTimer.set(STEP_DURATION);
}

void loop() {
  // click to place water
  if(buttonSingleClicked()) {
    FOREACH_FACE(f) {
      myStates[f] = WATER;
    }
  }
  
  // put your main code here, to run repeatedly:
  if (stepTimer.isExpired()) {
    stepTimer.set(STEP_DURATION);

    // update based on our state
    FOREACH_FACE(f) {
      switch (myStates[f]) {
        case WATER: waterLoop(f); break;
        case BEACH: beachLoop(f); break;
        case LAND: landLoop(f); break;
      }
    }
  }

  // display our state
  FOREACH_FACE(f) {
    switch (myStates[f]) {
      case WATER: setColorOnFace(CYAN, f); break;
      case BEACH: setColorOnFace(YELLOW, f); break;
      case LAND: setColorOnFace(GREEN, f); break;
    }
  }

  // share my individual face states
  FOREACH_FACE(f) {
    setValueSentOnFace(myStates[f], f);
  }
}

void init() {

  FOREACH_FACE(f) {
    // create 50% water
    // 25% beach
    // 25% land
    if (random(100) < 50) {
      myStates[f] = WATER;
    }
    else {
      if (random(100) < 50) {
        myStates[f] = BEACH;
      }
      else {
        myStates[f] = LAND;
      }
    }
  }
}

byte countNeigborsOfTypeForPosition(byte type, byte pos) {

  byte count = 0;

  // look at the 3 faces for this position
  byte neighborVal;
  // look at 2 positions neighboring me on this Blink
  byte pos_left = (pos + 5) % 6;
  byte pos_right = (pos + 1) % 6;

  if ( myStates[pos_left] == type ) {
    count++;
  }
  if ( myStates[pos_right] == type ) {
    count++;
  }
  // look at 1 position neighboring me on neighboring Blink
  if (!isValueReceivedOnFaceExpired(pos)) {
    byte neighborVal = getLastValueReceivedOnFace(pos);
    if (neighborVal == type) {
      count++;
    }
  }

  return count;
}

void waterLoop(byte face) {
  // if more than X neighbors are BEACH, then become a BEACH
  if (countNeigborsOfTypeForPosition(BEACH, face) > 1) {
    myStates[face] = BEACH;
  }
}

void beachLoop(byte face) {
  // If most neighbors are water, become water
  if (countNeigborsOfTypeForPosition(WATER, face) > 2 ) {
    myStates[face] = WATER;
  }

  // if NO neighbors are water, become land
  if (countNeigborsOfTypeForPosition(WATER, face) == 0) {
    myStates[face] = LAND;
  }
}

void landLoop(byte face) {

  //if ANY neighbors are water, become beach
  if (countNeigborsOfTypeForPosition(WATER, face) >= 1) {
    myStates[face] = BEACH;
  }
}

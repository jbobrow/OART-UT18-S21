/*
    Blinks Simulator by Move38

    Find out more about this project:
    https://github.com/Move38/Blinks-Simulator
    Click and drag blinks to re-arrange them.
    Click and drag in an open space to break them.
    You can use console for debugging info
*/

enum State {
  EMPTY,  // 0
  TREE,   // 1
  FIRE    // 2
};

Timer stepTimer;
#define STEP_DURATION 500

byte myState = EMPTY;

void setup() {
  randomize();
}

void loop() {

  if (buttonSingleClicked()) {
    myState = FIRE;
  }

  if (stepTimer.isExpired()) {
    stepTimer.set(STEP_DURATION);

    // DO STUFF AS EMPTY
    if (myState == EMPTY) {
      // 0.3% to become a Tree
      if (random(100) < 3) {
        myState = TREE;
      }
    }

    // DO STUFF AS A TREE
    else if (myState == TREE) {
      if (random(100) < 1) {
        myState = FIRE;
      }

      FOREACH_FACE(f) {
        if (!isValueReceivedOnFaceExpired(f)) {

          byte neighborVal = getLastValueReceivedOnFace(f);

          if (neighborVal == FIRE) {
            myState = FIRE;
          }
        }
      }
    }

    // DO STUFF AS FIRE
    else if (myState == FIRE) {
      myState = EMPTY;
    }
  }

  // Share our state
  setValueSentOnAllFaces(myState);

  // display myState
  switch (myState) {
    case EMPTY: setColor(OFF); break;
    case TREE: setColor(GREEN); break;
    case FIRE: setColor(ORANGE); break;
  }
}

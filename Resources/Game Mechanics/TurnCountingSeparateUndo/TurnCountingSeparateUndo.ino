/*
   Turn Counting Across All Blinks - Including movement

   In this example, the Blinks count up for a turn when
   a single Blink is clicked... or If Blinks are separated
   and then brought back together, this also counts as a turn.
   This is modelled for games like Fracture, or Trifoil

   Double Click goes back one move on all connected Blinks

*/

enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {TURN_START, TURN_END, GAME, RESET, SEPARATE, TOGETHER, UNDO};//these modes will simply be different colors
byte gameMode = GAME;//the default mode when the game begins

word turnCount = 0;

#define VIS_DURATION 200
Timer modeTimer;

//byte neighborsPresent[6];
byte numNeighbors;

void setup() {

}

void loop() {

  // The following listens for and updates game state across all Blinks
  switch (signalState) {
    case INERT:
      inertLoop();
      break;
    case GO:
      goLoop();
      break;
    case RESOLVE:
      resolveLoop();
      break;
  }

  // The following is loops for each of our game states
  switch (gameMode) {
    case TURN_START:
      turnStartLoop();
      break;
    case TURN_END:
      turnEndLoop();
      break;
    case GAME:
      gameLoop();
      break;
    case RESET:
      resetLoop();
      break;
    case SEPARATE:
      separateLoop();
      break;
    case TOGETHER:
      togetherLoop();
      break;
    case UNDO:
      undoLoop();
      break;
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData = (signalState << 4) + (gameMode);
  setValueSentOnAllFaces(sendData);

  if (buttonMultiClicked()) {
    changeMode(RESET);
  }
}

/*
   Mode 1
*/
void turnStartLoop() {

  // press to start
  if (modeTimer.isExpired()) {
    changeMode(TURN_END);
  }

  setColorOnFace(YELLOW, 0);
  setColorOnFace(YELLOW, 1);
  setColorOnFace(YELLOW, 2);
  setColorOnFace(OFF, 3);
  setColorOnFace(OFF, 4);
  setColorOnFace(OFF, 5);
}

/*
   Mode 2
*/
void turnEndLoop() {

  if (modeTimer.isExpired()) {
    changeMode(GAME);
  }

  setColorOnFace(OFF, 0);
  setColorOnFace(OFF, 1);
  setColorOnFace(OFF, 2);
  setColorOnFace(ORANGE, 3);
  setColorOnFace(ORANGE, 4);
  setColorOnFace(ORANGE, 5);
}

/*
   Mode 3
*/
void gameLoop() {

  if (buttonSingleClicked()) {
    changeMode(TURN_START);
  }

  if (buttonDoubleClicked()) {
    changeMode(UNDO);
  }


  // if we recently lost a neighbor, separate...
  if (getNumNeighbors() < numNeighbors) {
    changeMode(SEPARATE);
    numNeighbors = getNumNeighbors();
  }

  FOREACH_FACE(f) {
    if ((turnCount % 6) > f) {
      setColorOnFace(BLUE, f);
    }
    else {
      setColorOnFace(dim(BLUE, 64), f);
    }
  }
}

/*
   Separate Loop
*/
void separateLoop() {
  // if we recently gained a neighbor, together
  if (getNumNeighbors() > numNeighbors) {
    changeMode(TOGETHER);
    numNeighbors = getNumNeighbors();
  }

  setColorOnFace(YELLOW, 0);
  setColorOnFace(YELLOW, 1);
  setColorOnFace(YELLOW, 2);
  setColorOnFace(OFF, 3);
  setColorOnFace(OFF, 4);
  setColorOnFace(OFF, 5);
}

/*
   Together Loop
*/
void togetherLoop() {
  if (modeTimer.isExpired()) {
    changeMode(GAME);
  }

  setColorOnFace(OFF, 0);
  setColorOnFace(OFF, 1);
  setColorOnFace(OFF, 2);
  setColorOnFace(ORANGE, 3);
  setColorOnFace(ORANGE, 4);
  setColorOnFace(ORANGE, 5);
}

/*
   Reset Loop
*/

void resetLoop() {
  if (modeTimer.isExpired()) {
    changeMode(GAME);
  }
  setColor(dim(WHITE, 64));
}

/*
   Undo Loop
*/

void undoLoop() {
  if (modeTimer.isExpired()) {
    changeMode(GAME);
  }
  setColor(dim(GREEN, 128));
}


/*
   Get num neighbors
*/
byte getNumNeighbors() {
  byte count = 0;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      count++;
    }
  }
  return count;
}

/*
   pass this a game mode to switch to
*/
void changeMode( byte mode ) {
  byte prevMode = gameMode;
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors

  // handle any items that a game should do once when it changes
  if (mode != prevMode) {
    if (gameMode == TURN_START) {
      modeTimer.set( VIS_DURATION );
    }
    else if (gameMode == TURN_END) {
      // increment turn count
      turnCount++;
      modeTimer.set( VIS_DURATION );
    }
    else if (gameMode == SEPARATE) {
    }
    else if (gameMode == TOGETHER) {
      // increment turn count
      turnCount++;
      modeTimer.set( VIS_DURATION );
    }
    else if (gameMode == GAME) {
      numNeighbors = getNumNeighbors();
    }
    else if (gameMode == RESET) {
      turnCount = 0;
      modeTimer.set( VIS_DURATION );
    }
    else if (gameMode == UNDO) {
      turnCount--;
      modeTimer.set( VIS_DURATION );
    }
  }
}


/*
   This loop looks for a GO signalState
   Also gets the new gameMode
*/
void inertLoop() {

  //listen for neighbors in GO
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == GO) {//a neighbor saying GO!
        byte neighborGameMode = getGameMode(getLastValueReceivedOnFace(f));
        changeMode(neighborGameMode);
      }
    }
  }
}

/*
   If all of my neighbors are in GO or RESOLVE, then I can RESOLVE
*/
void goLoop() {
  signalState = RESOLVE;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not heard the GO news
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == INERT) {//This neighbor doesn't know it's GO time. Stay in GO
        signalState = GO;
      }
    }
  }
}

/*
   This loop returns me to inert once everyone around me has RESOLVED
   Now receive the game mode
*/
void resolveLoop() {
  signalState = INERT;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not moved to RESOLVE
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == GO) {//This neighbor isn't in RESOLVE. Stay in RESOLVE
        signalState = RESOLVE;
      }
    }
  }
}


byte getGameMode(byte data) {
  return (data & 7);//returns bits D, E and F
}

byte getSignalState(byte data) {
  return ((data >> 4) & 3);//returns bits A and B
}

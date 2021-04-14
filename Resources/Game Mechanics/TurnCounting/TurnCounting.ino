/*
   Turn Counting Across All Blinks

   In this example, the Blinks count up for a turn when
   a single Blink is clicked

*/

enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {TURN_START, TURN_END, GAME, RESET};//these modes will simply be different colors
byte gameMode = GAME;//the default mode when the game begins

word turnCount = 0;

Timer modeTimer;

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
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData = (signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);

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

  if (buttonMultiClicked()) {
    changeMode(RESET);
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
   Reset Loop
*/

void resetLoop() {
  if (modeTimer.isExpired()) {
    turnCount = 0;
    changeMode(GAME);
  }
  setColor(dim(WHITE, 64));
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
      modeTimer.set(300);
    }
    else if (gameMode == TURN_END) {
      // increment turn count
      turnCount++;
      modeTimer.set(300);
    }
    else if (gameMode == GAME) {
    }
    else if (gameMode == RESET) {
      modeTimer.set(300);
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
  return (data & 3);//returns bits E and F
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);//returns bits C and D
}

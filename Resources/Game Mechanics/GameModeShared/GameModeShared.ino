/*
   Shared Game Mode Across All Blinks
   This can work for as many different game modes as you need
   Switching between them simply requires a call to changeMode()
   The signalState takes care of making sure all other Blinks change too
*/

enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {MODE1, MODE2, MODE3};//these modes will simply be different colors
byte gameMode = MODE1;//the default mode when the game begins

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
    case MODE1:
      mode1Loop();
      break;
    case MODE2:
      mode2Loop();
      break;
    case MODE3:
      mode3Loop();
      break;
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData = (signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);
}

/*
 * Mode 1
 */
void mode1Loop() {

  if(buttonSingleClicked()) {
    changeMode(MODE2);
  }
  
  setColor(RED);
}

/*
 * Mode 2
 */
void mode2Loop() {

  if(buttonSingleClicked()) {
    changeMode(MODE3);
  }
  
  setColor(YELLOW);
}

/*
 * Mode 3
 */
void mode3Loop() {

  if(buttonSingleClicked()) {
    changeMode(MODE1);
  }
  
  setColor(BLUE);
}


/*
 * pass this a game mode to switch to
 */
void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors 
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
        signalState = GO;
        gameMode = getGameMode(getLastValueReceivedOnFace(f));
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

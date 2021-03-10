/*
   Shared Game Mode Across All Blinks
   This can work for as many different game modes as you need
   Switching between them simply requires a call to changeMode()
   The signalState takes care of making sure all other Blinks change too

   In this example, the game mode, MODE2, counts down
   and when the timer is expired, the game mode is changed
   to end game. You could also have a game action end the game early.
   The change game function looks to see if there is anything special
   we should do when changing game states, i.e. starting the timer.
   We can't simply do this where the button press is received, because
   only one of the Blinks is receiving the button press, so it makes
   sense to start the timer on the act of changing game states
*/

enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {MODE1, MODE2, MODE3};//these modes will simply be different colors
byte gameMode = MODE1;//the default mode when the game begins

Timer gameTimer;
#define GAME_DURATION 6000 // 6 seconds

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
   Mode 1
*/
void mode1Loop() {

  // press to start
  if (buttonPressed()) {
    changeMode(MODE2);  // change game mode on all Blinks
  }

  setColor(RED);
}

/*
   Mode 2
*/
void mode2Loop() {

  // map the time remaining from 0 - 6000 milliseconds to the value 0-6
  byte timeRemaining = map(gameTimer.getRemaining(), 0, GAME_DURATION, 0, 6);

  // display how much time is left in the game (0-6 LEDs on)
  FOREACH_FACE(f) {
    if (f < timeRemaining) {
      setColorOnFace(YELLOW, f);
    }
    else {
      setColorOnFace(OFF, f);
    }
  }

  if (gameTimer.isExpired()) {
    changeMode(MODE3);
  }

  // check and dump button pressed during gameplay
  buttonPressed();
}

/*
   Mode 3
*/
void mode3Loop() {

  if (buttonPressed()) {
    changeMode(MODE1);
  }

  setColor(BLUE);
}


/*
   pass this a game mode to switch to
*/
void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors

  // handle any items that a game should do once when it changes
  if (gameMode == MODE1) {
    gameTimer.never(); // set the game timer to never expire
  }
  else if (gameMode == MODE2) {
    gameTimer.set(GAME_DURATION); // start a game timer
  }
  else if (gameMode == MODE3) {
    gameTimer.set(0); // end the game timer (just in case)
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

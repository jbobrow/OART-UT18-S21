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

enum gameModes {GAME, FAIL, SUCCESS};//these modes will simply be different colors
byte gameMode = GAME;//the default mode when the game begins

//game stuff

//________
//fade timers
Timer dimTimer;
#define DIM_TIME 8000
//_______
//shape recognition
int neighborCount = 0;
//_______
//color changing
Color cycleColors[4] = {RED, GREEN, YELLOW, ORANGE};
int cycleCount = 0;
bool increment = false;


//_____



//fail and success variables
int clicks = 0;
int between = 10;
Timer fadeTimer;
bool turnOff = false;



void setup() {
  dimTimer.set(DIM_TIME);
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
    case GAME:
      gameLoop();
      break;
    case FAIL:
      failLoop();
      break;
    case SUCCESS:
      successLoop();
      break;
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData =(signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);
}

/*
   Mode 1
*/
void gameLoop() {


  int fadeTime = dimTimer.getRemaining();

  //map the brightness in order to use it
  byte brightness = map(fadeTime, 0, 8000, 0, 255);

  //if I single click a button, restore the button
  if (buttonSingleClicked())
  {
    dimTimer.set(DIM_TIME);
  }


  //count how many neighbors I have but first reset variables every passthrough

  //RESET VARIABLES
  neighborCount = 0;

  FOREACH_FACE(f)
  {
    if (!isValueReceivedOnFaceExpired(f))
    {
      neighborCount++;
    }
  }

  //if I have 5 neighbors, I win
  if (neighborCount == 5 && signalState == INERT)
  {
    changeMode(SUCCESS);
  }

  //switch the color if im alone

  if (isAlone())
  {
    if (increment == true)
    {
      cycleCount++;
      cycleCount = cycleCount % 4;
      increment = false;
    }
  }
  else
  {
    increment = true;
  }



  //display logic

  if (dimTimer.isExpired() == true)
  {
    changeMode(FAIL);
  }
  else
  {
    setColor(dim(WHITE, brightness));
  }


}

/*
   Mode 2
*/
void failLoop() {

  setColor(dim(RED, 150));
  setColorOnFace(RED, (millis() / 1000) % 6);

  //if a blink is single clicked, play game again
  if (buttonSingleClicked())
  {
    changeMode(GAME);
  }
}

/*
   Mode 3
*/
void successLoop() {

  //display logic

  setColor(dim(GREEN, 150));
  setColorOnFace(GREEN, (millis() / 450) % 6);

  //if a button is pressed, play game again
  if (buttonSingleClicked())
  {
    changeMode(GAME);
  }
}


/*
   pass this a game mode to switch to
*/
void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors

  // handle any items that a game should do once when it changes
  if (gameMode == GAME) {
    dimTimer.set(DIM_TIME);
  }
  else if (gameMode == FAIL) {
    dimTimer.never();
  }
  else if (gameMode == SUCCESS) {
    dimTimer.never();
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

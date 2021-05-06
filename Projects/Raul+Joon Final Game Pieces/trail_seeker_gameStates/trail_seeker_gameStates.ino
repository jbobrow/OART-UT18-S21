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

enum gameModes {GAME, WIN, RESET};
byte gameMode = WIN;//the default mode when the game begins

enum tileStates {GRASS, BUNNY, FOX, TRAIL};
byte tileState = GRASS;

Timer gameTimer;
#define GAME_DURATION 6000 // 6 seconds

//GAME LOGIC
bool isBunny = false;
bool isBunnyTrail = false;
int pieceCycle = 0;
bool isFox = false;
bool isGrass = false;
bool transition = false;
bool hideBunny = false;
bool reset = false;
bool animalTransition = false;



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
    case GAME:
      gameLoop();
      break;
    case WIN:
      winLoop();
      break;
    case RESET:
      resetLoop();
      break;
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData = (tileState << 4) + (signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);
}

/*
   Mode 1
*/
void gameLoop() {

  // press to start
  if (buttonDoubleClicked())
  {
    pieceCycle++;
  }
  if (((pieceCycle) % 3) == 0 || isGrass == true || tileState == GRASS)
  {
    //if these conditions are true, then become grass and not anything else
    isGrass = true;
    isBunny = false;
    isFox = false;
    pieceCycle = 0;
    tileState = GRASS;
  }

  if (((pieceCycle) % 3) == 1 || isBunny == true || tileState == BUNNY)
  {
    //if these conditions are true, then become bunny and not anything else
    isGrass = false;
    isBunny = true;
    isFox = false;
    pieceCycle = 1;
    tileState = BUNNY;
  }
  if (((pieceCycle) % 3) == 2 || isFox == true || tileState == FOX)
  {
    //if these conditions are true, then become a fox and not anything else
    isGrass = false;
    isBunny = false;
    isFox = true;
    pieceCycle = 2;
    tileState = FOX;
  }

  if (tileState == GRASS)
  {
    //grass display
    setColor(dim(GREEN, 135));
    setColorOnFace(GREEN, ((millis() / 1000) % 6));
  }

  if (tileState == BUNNY)
  {
    //bunny display and messaging
    setColor(dim(WHITE, 125));
    setColorOnFace(WHITE, ((millis() / 200) % 6));
    //setValueSentOnAllFaces(3);
  }
  if (tileState == TRAIL)
  {
    //dimmer white to display the bunny trail
    setColor(dim(WHITE, 120));
  }
  if (tileState == FOX)
  {
    //fox display
    setColor(dim(ORANGE, 125));
    setColorOnFace(ORANGE, ((millis() / 500) % 6));
    //setValueSentOnAllFaces(4);
  }

  //_________ CHANGING LOGIC

  if (buttonSingleClicked())
  {
    //If I single click a non-animal element, then tell it to transition
    if (tileState == TRAIL || tileState == GRASS)
    {
      transition = true;
    }
    //if I single clicked an animal, then transition in another specific way
    if (tileState == BUNNY || tileState == FOX)
    {
      animalTransition = true;
    }

  }

  //wipe single clicks
  buttonSingleClicked();

  if (transition)
  {
    //send a value to distinguish transition
    setValueSentOnAllFaces(5);

    FOREACH_FACE(f)
    {
      //change tile [to move to] to bunny
      if (myTileState(getLastValueReceivedOnFace(f)) == BUNNY && tileState == GRASS && !isValueReceivedOnFaceExpired(f))
      {
        isBunny = true;
        tileState = BUNNY;
        pieceCycle = 1;
      }
      //if I recieve the signal from a fox to change and I am grass, change to fox
      if (myTileState(getLastValueReceivedOnFace(f)) == FOX && tileState == GRASS && !isValueReceivedOnFaceExpired(f))
      {
        isFox = true;
        tileState = FOX;
        pieceCycle = 2;
      }
      //if I recieve the signal from a fox to change and I am grass, change to fox
      if (myTileState(getLastValueReceivedOnFace(f)) == FOX && tileState == TRAIL && !isValueReceivedOnFaceExpired(f))
      {
        isFox = true;
        tileState = FOX;
        pieceCycle = 2;
      }

    }
    //close both gates after movement has happened
    transition = false;
    animalTransition = false;
  }


  if (animalTransition)
  {
    //If i am an animal, send specific values for my type
    if (isBunny)
    {
      setValueSentOnAllFaces(3);
    }
    if (isFox)
    {
      setValueSentOnAllFaces(4);
    }

    FOREACH_FACE(f)
    {
      //if i was a a bunny blink and i am receiving a signal to transition from grass, show trail but don't be a bunny
      if (myTileState(getLastValueReceivedOnFace(f)) == GRASS && tileState == BUNNY && !isValueReceivedOnFaceExpired(f))
      {
        isBunnyTrail = true;
        tileState = TRAIL;
        isBunny = false;
      }
      //if I am a bunny and i recieve a fox signal, turn to fox and trigger a win state
      if (myTileState(getLastValueReceivedOnFace(f)) == FOX && tileState == FOX && !isValueReceivedOnFaceExpired(f))
      {
        isFox = true;
        pieceCycle = 2;
        //TRIGGER THE WIN STATE
        changeMode(WIN);
      }
      //if I was a fox and I am told to transition, turn off
      if (myTileState(getLastValueReceivedOnFace(f)) == GRASS && tileState == FOX && !isValueReceivedOnFaceExpired(f))
      {
        isGrass = true;
        tileState = GRASS;
        pieceCycle = 0;
      }
      //if i am a fox and I recieve a bunny signal, turn off
      if (myTileState(getLastValueReceivedOnFace(f)) == BUNNY && tileState == FOX && !isValueReceivedOnFaceExpired(f))
      {
        isGrass = true;
        tileState = GRASS;
        pieceCycle = 0;
      }
    }
  }

  //__HIDING RABBIT STATE

  //If the button is triple clicked, change the bunny to match the surroundings
  if (buttonMultiClicked() && isBunny == true)
  {
    hideBunny = !hideBunny;
  }
  if (hideBunny)
  {
    setColor(dim(GREEN, 135));
    setColorOnFace(GREEN, ((millis() / 1000) % 6));
  }

  //RESET

  if (buttonLongPressed())
  {
    changeMode(RESET);
  }

}

/*
   Mode 2
*/
void winLoop() {
  setColor(dim(ORANGE, 125));
  setColorOnFace(ORANGE, ((millis() / 90) % 6));
  if (buttonPressed())
  {
    isGrass = true;
    pieceCycle = 0;
    changeMode(GAME);
  }
}

/*
   Mode 3
*/
void resetLoop() {

  if (buttonPressed()) {
    pieceCycle = 0;
    isGrass = true;
    changeMode(GAME);
  }

  setColor(OFF);
}


/*
   pass this a game mode to switch to
*/
void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors

  // handle any items that a game should do once when it changes
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

byte myTileState(byte data) {
  return ((data >> 4) & 3);//returns bits A and B
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);//returns bits C and D
}

byte getGameMode(byte data) {
  return (data & 3);//returns bits E and F
}

//note to reader- double click to cycle through possibilities. To move, click on your  blink and click on a 
//neighboring blink

enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {GAME, WIN, RESET};
byte gameMode = GAME;//the default mode when the game begins

//NOTE FROM RS- IF I LONG PRESS OR IF I JUMP A FOX ONTO A BUNNY, THE CORRESPONDING STATE
//DOESN'T TRANSMIT TO ALL THE BLINKS. HOWEVER, IF YOU CHANGE THE gameMode to WIN or RESET AND SINGLE CLICK
//IT BRINGS ALL THE BLINKS TO GREEN AND WORKS FINE

enum tileStates {GRASS, BUNNY, FOX, TRAIL};
byte tileState = GRASS;

//GAME LOGIC
//bool isBunny = false;
//bool isBunnyTrail = false;
//bool isFox = false;
//bool isGrass = false;
int pieceCycle = 0;
bool transition = false;
bool hideBunny = false;
bool reset = false;
bool animalTransition = false;
bool transitionDone = false;



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
  // share both signalState (i.e. when to change) and the game mode and also what I am
  //byte sendData = (tileState << 4) + (signalState << 2) + (gameMode);
  //setValueSentOnAllFaces(sendData);


  //If i'm not transitioning, nobody needs to know what I am
  if (transition == false && animalTransition == false)
  {
    byte sendData = (signalState << 2) + (gameMode);
    setValueSentOnAllFaces(sendData);
  }

}

/*
   Mode 1
*/
void gameLoop() {

  //_____CYCLING THROUGH AND DISPLAYING POSIBILITIES
  
  // double click to cycle through the possibilities of the tiles
  if (buttonDoubleClicked())
  {
    pieceCycle++;
  }

  //start off on grass
  if (((pieceCycle) % 3) == 0)// || tileState == GRASS)
  {
    pieceCycle = 0;
    tileState = GRASS;
  }
  if (tileState == GRASS)
  {
    //grass display
    setColor(dim(GREEN, 135));
    setColorOnFace(GREEN, ((millis() / 1000) % 6));
  }

  //one up in the cycle is a bunny
  if (((pieceCycle) % 3) == 1)// || tileState == BUNNY)
  {
    //if these conditions are true, then become bunny and not anything else
    pieceCycle = 1;
    tileState = BUNNY;
  }

  if (tileState == BUNNY)
  {
    //bunny display
    setColor(dim(WHITE, 125));
    setColorOnFace(WHITE, ((millis() / 200) % 6));
  }

  //two up in the cycle is a fox
  if (((pieceCycle) % 3) == 2)// || tileState == FOX)
  {
    //if these conditions are true, then become a fox and not anything else
    pieceCycle = 2;
    tileState = FOX;
  }
  if (tileState == FOX)
  {
    //fox display
    setColor(dim(ORANGE, 125));
    setColorOnFace(ORANGE, ((millis() / 500) % 6));
  }

  if (tileState == TRAIL)
  {
    //dimmer white to display the bunny trail
    setColor(dim(WHITE, 120));
  }


  //_________ CHANGING LOGIC

  if (buttonSingleClicked())
  {
    //If I single click a non-animal tile, then tell it to transition
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

  //NON-ANIMAL TRANISTION
  if (transition)
  {
    //send what I am when I want to transition
    byte sendData = (tileState << 4) + (signalState << 2) + (gameMode);
    setValueSentOnAllFaces(sendData);

    FOREACH_FACE(f)
    {
      //if I am grass and I get the signal from a bunny to change, turn into a bunny
      if (myTileState(getLastValueReceivedOnFace(f)) == BUNNY && tileState == GRASS && !isValueReceivedOnFaceExpired(f))
      {
        tileState = BUNNY;
        pieceCycle = 1;
      }
      //if I recieve the signal from a fox to change and I am grass, change to fox
      if (myTileState(getLastValueReceivedOnFace(f)) == FOX && tileState == GRASS && !isValueReceivedOnFaceExpired(f))
      {
        tileState = FOX;
        pieceCycle = 2;
      }
      //if I recieve the signal from a fox to change and I am trail, change to fox
      if (myTileState(getLastValueReceivedOnFace(f)) == FOX && tileState == TRAIL && !isValueReceivedOnFaceExpired(f))
      {
        tileState = FOX;
        pieceCycle = 2;
      }

    }
    //close both gates after movement has happened
    //transition = false;
    //animalTransition = false;
    //____

    //if I have finished transitioning as a non-animal, say that the transition is done
    transitionDone = true;
  }

  //ANIMAL TRANSITION
  if (animalTransition)
  {
    /*//If i am an animal, send specific values for my type
    if (tileState == BUNNY)
      {
      setValueSentOnAllFaces(3);
      }
      if (tileState == FOX)
      {
      setValueSentOnAllFaces(4);
      }*/
      
    //if I am transitioning as an animal, tell my neighbors what I am
    byte sendData = (tileState << 4) + (signalState << 2) + (gameMode);
    setValueSentOnAllFaces(sendData);

    FOREACH_FACE(f)
    {
      //if i was a a bunny blink and i am receiving a signal to transition from grass, show trail but don't be a bunny
      if (myTileState(getLastValueReceivedOnFace(f)) == GRASS && tileState == BUNNY && !isValueReceivedOnFaceExpired(f))
      {
        tileState = TRAIL;
      }
      //if I am a bunny and i recieve a fox signal, turn to fox and trigger a win state
      if (myTileState(getLastValueReceivedOnFace(f)) == FOX && tileState == BUNNY && !isValueReceivedOnFaceExpired(f))
      {
        tileState = FOX;
        //TRIGGER THE WIN STATE
        changeMode(WIN);
      }
      //if I was a fox and I am told to transition by grass, turn off
      if (myTileState(getLastValueReceivedOnFace(f)) == GRASS && tileState == FOX && !isValueReceivedOnFaceExpired(f))
      {
        tileState = GRASS;
      }
      //if I was a fox and I am told to transition by trail, turn off
      if (myTileState(getLastValueReceivedOnFace(f)) == TRAIL && tileState == FOX && !isValueReceivedOnFaceExpired(f))
      {
        tileState = GRASS;
        pieceCycle = 0;
      }
      //if i am a fox and I recieve a bunny signal, turn off
      if (myTileState(getLastValueReceivedOnFace(f)) == BUNNY && tileState == FOX && !isValueReceivedOnFaceExpired(f))
      {
        tileState = GRASS;
        pieceCycle = 0;
      }
    }
  }

  //if both are finished transitioning and checking, no need to check anymore
  if (transitionDone)
  {
    transition = false;
    animalTransition = false;
    transitionDone = false;
  }

  //__HIDING RABBIT STATE

  //If the button is triple clicked, change the bunny to match the surroundings
  if (buttonMultiClicked() && tileState == BUNNY)
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

  //go crazy with orange
  setColor(dim(ORANGE, 125));
  setColorOnFace(ORANGE, ((millis() / 90) % 6));
  if (buttonPressed())
  {
    tileState = GRASS;
    changeMode(GAME);
  }
}

/*
   Mode 3
*/
void resetLoop() {

  if (buttonPressed()) {
    tileState = GRASS;
    changeMode(GAME);
  }

  //turn off and change
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

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
Timer wipeTimer;



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
  //byte sendData = + (signalState << 2) + (gameMode);
  //setValueSentOnAllFaces(sendData);


  //If i'm not transitioning, nobody needs to know what I am

   byte sendData =  (tileState << 4) + (signalState << 2) + (gameMode);
   setValueSentOnAllFaces(sendData);
  
}

/*
   Mode 1
*/
void gameLoop() {

  //_____CYCLING THROUGH AND DISPLAYING POSIBILITIES

  // double click to cycle through the possibilities of the tiles
  if (buttonDoubleClicked() && tileState != TRAIL)
  {
    pieceCycle++;
    if(pieceCycle == 3)
    {
     pieceCycle = 0; 
    }
  }

  //start off on grass
  if (((pieceCycle) % 4) == 0)// || tileState == GRASS)
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
  if (((pieceCycle) % 4) == 1)// || tileState == BUNNY)
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
  if (((pieceCycle) % 4) == 2)// || tileState == FOX)
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
  
  if(((pieceCycle) % 4) == 3)
  {
    pieceCycle = 3;
    tileState = TRAIL;
  }

  //TRAIL DISPLAY
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
    changeTile(tileState);
  }

  //wipe single clicks
  buttonSingleClicked();

  //ANIMAL TRANSITION
  if (animalTransition)
  {

    //if I am transitioning as an animal, tell my neighbors what I am

    FOREACH_FACE(f)
    {
      byte neighborTileState = getTileState(getLastValueReceivedOnFace(f));
      //if i was a a bunny blink and i am receiving a signal to transition from grass, show trail but don't be a bunny
      byte neighborSignal = getSignalState(getLastValueReceivedOnFace(f));
        
      if (!isValueReceivedOnFaceExpired(f) && neighborSignal == GO)
      {
        if (neighborTileState == BUNNY && tileState == BUNNY)
        {
          tileState = TRAIL;
          pieceCycle = 3;
          signalState = RESOLVE;
          
        }
        //if I am a bunny and i recieve a fox signal, turn to fox and trigger a win state
        else if (neighborTileState == FOX && tileState == BUNNY)
        {
          tileState = FOX;
          pieceCycle = 2;
          //TRIGGER THE WIN STATE
          changeMode(WIN);
        }
        //if I was a fox and I am told to transition by grass, turn off
        else if (neighborTileState == FOX && tileState == FOX)
        {
          tileState = GRASS;
          pieceCycle = 0;
          signalState = RESOLVE;
        }
        //if I was a fox and I am told to transition by trail, turn off
        else if (neighborTileState == TRAIL && tileState == FOX)
        {
          tileState = GRASS;
          pieceCycle = 0;
          signalState = RESOLVE;
        }
        //if i am a fox and I recieve a bunny signal, turn off
        else if (neighborTileState == BUNNY && tileState == FOX )
        {
          tileState = GRASS;
          pieceCycle = 0;
          signalState = RESOLVE;
        }
        break;
      }
    }
  }

  //NON-ANIMAL TRANISTION
  if (transition)
  {
    //send what I am when I want to transition
    //byte sendData = (tileState << 4) + (signalState << 2) + (gameMode);
    //setValueSentOnAllFaces(sendData);

    FOREACH_FACE(f)
    {
      byte animalNeighborState = getTileState(getLastValueReceivedOnFace(f));
      byte neighborSignal = getSignalState(getLastValueReceivedOnFace(f));
      
      //if I am grass and I get the signal from a bunny to change, turn into a bunny
      if (!isValueReceivedOnFaceExpired(f) && neighborSignal == GO)
      {

        //if I recieve the signal from a fox to change and I am grass, change to fox
        if (animalNeighborState == FOX && tileState == GRASS)
        {
          tileState = FOX;
          pieceCycle = 2;
          signalState = RESOLVE;
        }        
        else if (animalNeighborState == BUNNY && tileState == GRASS)
        {
          tileState = BUNNY;
          pieceCycle = 1;
          signalState = RESOLVE; // IN QUESTION
        }
        //if I recieve the signal from a fox to change and I am trail, change to fox
        else if (animalNeighborState == FOX && tileState == TRAIL)
        {
          tileState = FOX;
          pieceCycle = 2;
          signalState = RESOLVE;
        }
        break;
      }

    }
    //say that the transition is done
    transitionDone = true;
  }



  //if both are finished transitioning and checking, no need to check anymore
  if (transitionDone)
  {
    changeMode(GAME);
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

  //RESET A SINGLE BLINK BY LONG PRESSING IT

  if (buttonLongPressed())
  {
    gameMode = RESET;
  }

}

/*
   Mode 2
*/
void winLoop() {

  //go crazy with red
  setColor(dim(RED, 125));
  setColorOnFace(RED, ((millis() / 60) % 6));

  if (buttonSingleClicked())
  {
    changeMode(RESET);
  }
}

/*
   Mode 3
*/
void resetLoop() {

   //turn off and change
   //wipeTimer.set(1000);
 setColor(OFF);
  //if (wipeTimer.isExpired()) {
    tileState = GRASS;
    pieceCycle = 0;
    changeMode(GAME);
 // }


}


/*
   pass this a game mode to switch to
*/
void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors

  // handle any items that a game should do once when it changes
}

void changeTile(byte state)
{
  signalState = GO;
  tileState = state;
}

/*
   This loop looks for a GO signalState
   Also gets the new gameMode
*/
void inertLoop() {

  //listen for neighbors in GO
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == GO && getGameMode(getLastValueReceivedOnFace(f)) != GAME) {//a neighbor saying GO!
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
      if (getSignalState(getLastValueReceivedOnFace(f)) == RESOLVE && gameMode == GAME)
      {
        signalState = RESOLVE;
        break;
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

byte getTileState(byte data) {
  return ((data >> 4) & 3);//returns bits A and B
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);//returns bits C and D
}

byte getGameMode(byte data) {
  return (data & 3);//returns bits E and F
}

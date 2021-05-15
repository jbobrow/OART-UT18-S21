
enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {GAME, WIN, RESET};
byte gameMode = GAME;//the default mode when the game begins

//NOTE FROM RS- IF I LONG PRESS OR IF I JUMP A FOX ONTO A BUNNY, THE CORRESPONDING STATE
//DOESN'T TRANSMIT TO ALL THE BLINKS. HOWEVER, IF YOU CHANGE THE gameMode to WIN or RESET AND SINGLE CLICK
//IT BRINGS ALL THE BLINKS TO GREEN AND WORKS FINE

//possible tiles
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


  //constantly send my information (what I am, my signal state and game mode)

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

    //if I exceed a piece cycle of two, loop piece cycle (cycle between grass, bunny, and fox)
    if (pieceCycle == 3)
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
  //grass display
  if (tileState == GRASS)
  {
    setColor(dim(GREEN, 135));
    setColorOnFace(GREEN, ((millis() / 1000) % 6));
  }

  //one up in the cycle is a bunny
  if (((pieceCycle) % 4) == 1)// || tileState == BUNNY)
  {
    pieceCycle = 1;
    tileState = BUNNY;
  }
  //bunny display
  if (tileState == BUNNY)
  {
    setColor(dim(WHITE, 125));
    setColorOnFace(WHITE, ((millis() / 200) % 6));
  }

  //two up in the cycle is a fox
  if (((pieceCycle) % 4) == 2)// || tileState == FOX)
  {
    pieceCycle = 2;
    tileState = FOX;
  }
  //fox display
  if (tileState == FOX)
  {
    setColor(dim(ORANGE, 125));
    setColorOnFace(ORANGE, ((millis() / 500) % 6));
  }

  //if I am 3 in the piece cycle, turn into a trail in order to not conflict when changing to a trail
  if (((pieceCycle) % 4) == 3)
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

    //signal that I want to change
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

      //get my neighbor's tile state
      byte neighborTileState = getTileState(getLastValueReceivedOnFace(f));

      //get my neighbor's signal state
      byte neighborSignal = getSignalState(getLastValueReceivedOnFace(f));

      if (!isValueReceivedOnFaceExpired(f) && neighborSignal == GO) // If I have a neighbor and they are telling me they are transitioning
      {
        if (neighborTileState == BUNNY && tileState == BUNNY) // & they have changed to a bunny and I am a bunny
        {
          //change to a trail and say I am done transitioning
          tileState = TRAIL;
          pieceCycle = 3;
          signalState = RESOLVE;

        }
        // & they have changed to a fox and I am a bunny
        else if (neighborTileState == FOX && tileState == BUNNY)
        {
          //set myself to a fox
          tileState = FOX;
          pieceCycle = 2;
          //TRIGGER THE WIN STATE
          changeMode(WIN);
        }
        //& they have changed to a fox and I am a fox
        else if (neighborTileState == FOX && tileState == FOX)
        {
          //leave a grass piece and say I am done transitioning
          tileState = GRASS;
          pieceCycle = 0;
          signalState = RESOLVE;
        }
        //& they have changed to a trail and I am a fox
        else if (neighborTileState == TRAIL && tileState == FOX)
        {
          //become a grass piece and say I am done transitioning
          tileState = GRASS;
          pieceCycle = 0;
          signalState = RESOLVE;
        }
        //& they are a Bunny and I am a fox
        else if (neighborTileState == BUNNY && tileState == FOX )
        {
          //leave a grass piece and say I am done transitioning
          tileState = GRASS;
          pieceCycle = 0;
          signalState = RESOLVE;
        }
        //break out after something has happened
        break;
      }
    }
  }

  //NON-ANIMAL TRANISTION
  if (transition)
  {

    FOREACH_FACE(f)
    {
      //get my neighbor's tile state and signal state on every face chach
      byte animalNeighborState = getTileState(getLastValueReceivedOnFace(f));
      byte neighborSignal = getSignalState(getLastValueReceivedOnFace(f));

      //if I have a neighbor and they are saying to transition
      if (!isValueReceivedOnFaceExpired(f) && neighborSignal == GO)
      {
        //& my neighbor is a fox and I am grass
        if (animalNeighborState == FOX && tileState == GRASS)
        {
          //become a fox and say I am done transitioning
          tileState = FOX;
          pieceCycle = 2;
          signalState = RESOLVE;
        }
        //& my neighbor is a bunny and I am grass
        else if (animalNeighborState == BUNNY && tileState == GRASS)
        {
          //become a bunny and say I am done transitioning
          tileState = BUNNY;
          pieceCycle = 1;
          signalState = RESOLVE; // IN QUESTION
        }

        //& I am a trail and and my neighbor is a fox
        else if (animalNeighborState == FOX && tileState == TRAIL)
        {
          //become a fox and say I am done transitioning
          tileState = FOX;
          pieceCycle = 2;
          signalState = RESOLVE;
        }
        //break out after a loop
        break;
      }

    }
    //say that the transition is done
    transitionDone = true;
  }



  //if both are finished transitioning and checking, no need to check anymore
  if (transitionDone)
  {
    changeMode(GAME);//finish all transitiongs
    //all transitions are done
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

  setColor(OFF);
  tileState = GRASS;
  pieceCycle = 0;
  changeMode(GAME);


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
  signalState = GO; //signal to my neighbor that I want to change
  tileState = state; //register my state
}

/*
   This loop looks for a GO signalState
   Also gets the new gameMode
*/
void inertLoop() {

  //listen for neighbors in GO
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      //a neighbor saying GO and my neighbor is not in game
      if (getSignalState(getLastValueReceivedOnFace(f)) == GO && getGameMode(getLastValueReceivedOnFace(f)) != GAME) {
        byte neighborGameMode = getGameMode(getLastValueReceivedOnFace(f)); //register neighbor game mode and
        //change to the game mode of my neighbor
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
      //If neighbor is in resolve and the game mode is in game(happens after a transition), switch me to resolve and break out
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

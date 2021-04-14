//game mode switch
enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {SELECT_TEAMS, JACKPOT_SELECT, GAME, JACKPOT_WIN, TIME_UP};//these modes will simply be different colors
byte gameMode = SELECT_TEAMS;//the default mode when the game begins


// team select
Color teamColors[3] = {BLUE, CYAN, ORANGE};
byte color = 0;

//jackpot selection
bool jackpot = false;
byte pointValue;

#define NO_FACE 255

//gameplay
Timer gameTimer;
#define GAME_DURATION 10000
bool isThrower = false;
bool isSpinning = true;
int throwerAttachPosition = NO_FACE;
int jackpotFacePosition = 0;
int twoPointFacePosition = 0;
int onePointFacePosition = 0;



void setup() {

  randomize();
  pointValue = random(1);
  gameTimer.never();

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
    case SELECT_TEAMS:
      selectTeamLoop();
      break;
    case JACKPOT_SELECT:
      selectJackpotLoop();
      break;
    case GAME:
      gameLoop();
      break;
    case JACKPOT_WIN:
      jackpotWinLoop();
      break;
    case TIME_UP:
      timeUpLoop();
      break;
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData = (isThrower << 4) + (signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);

  // clear all button presses if not used
  buttonSingleClicked();
  buttonDoubleClicked();
  buttonMultiClicked();
}

/*
   Mode 1-team select
*/
void selectTeamLoop() {
  //change mode
  if (buttonDoubleClicked()) {
    changeMode(JACKPOT_SELECT);
  }

  // change back to a blank slate if accidentaly clicked
  if (buttonMultiClicked())
  {
    isThrower = false;
  }

  //set up a team blink
  if (buttonSingleClicked())
  {
    color = (color + 1) % 3;
    isThrower = true;
  }

  // display states
  if (isThrower) {
    setColor(teamColors[color]);
  }
  else {
    // display white if not a thrower and in Mode 1
    setColor(WHITE);
  }
}

/*
   Mode 2- jackpot select
*/
void selectJackpotLoop() {

  if (isThrower == false)
  {
    setColor(OFF);

    // only allow setting jackpots on spinners
    //set the jackpot blink
    if (buttonSingleClicked())
    {
      jackpot = !jackpot; // toggles jackpot on and off
    }
    if (jackpot == true)
    {
      setColorOnFace(YELLOW, (millis() / 250) % 6);
    }
  }

  //change mode to game play
  if (buttonDoubleClicked()) {
    changeMode(GAME);
  }


}

/*
   Mode 3- actual gameplay
*/
void gameLoop() {
  //Timer started in changeMode

  //implement thrower status

  if (isThrower == true)
  {
    setColor(teamColors[color]);
    gameTimer.never();
  }

  //display the arrangement

  //just setting face positions for the 3 different types of blinks
  if (isThrower == false && isSpinning == true)
  {
    jackpotFacePosition = (millis() / 75) % 6;
    twoPointFacePosition = (millis() / 250) % 6;
    onePointFacePosition = (millis() / 500) % 6;
  }

  //displaying the different types of blinks
  if (isSpinning == true)
  {
    if (jackpot == true )
    {
      setColor(dim(YELLOW, 125));
      setColorOnFace(YELLOW, jackpotFacePosition);
      //isSpinning = true;
    }
    if (pointValue == 1 && jackpot == false && isThrower == false) //worth 2 points
    {
      setColor(dim(MAGENTA, 125));
      setColorOnFace(MAGENTA, twoPointFacePosition);
      //isSpinning = true;
    }
    if (pointValue == 0 && jackpot == false && isThrower == false) //worth 1 point
    {
      setColor(dim(BLUE, 75));
      setColorOnFace(CYAN, onePointFacePosition);
      //isSpinning = true;
    }
  }



  //check attatchments
  if (isThrower == false)
  {
    //if its not a thrower, reset its had a thrower attached to a face
    throwerAttachPosition = NO_FACE;

    FOREACH_FACE(f)
    {
      if (!isValueReceivedOnFaceExpired(f)) //if I have a neighbor and it is a thrower
      {
        if (getThrowerStatus(getLastValueReceivedOnFace(f))) { // if my neighbor is a thrower
          // record the position of the throwers attachment
          throwerAttachPosition = f;
        }
      }
    }

    //if a thrower has been attached, stop the spinning of that blink
    if (throwerAttachPosition != NO_FACE)
    {
      //stop spinning
      isSpinning = false;
    }
    else
    {
      //keep spinning
      isSpinning = true;
    }
  }

  //if a blink is a spinner
  if (isThrower == false)
  {
    //and its not spinning
    if (isSpinning == false)
    {
      if (throwerAttachPosition == jackpotFacePosition && jackpot == true)
      {
        //send to jackpot win
        changeMode(JACKPOT_WIN);
      }
      //send to whichever type of blink its connected to (2 pt or 1 pt)
      if (throwerAttachPosition == twoPointFacePosition && pointValue == 1 && jackpot == false)
      {
        setColor(GREEN);
        gameTimer.never();
      }
      if (throwerAttachPosition == onePointFacePosition && pointValue == 0 && jackpot == false)
      {
        setColor(GREEN);
        gameTimer.never();
      }

    }
  }


  //switch game modes

  //if the timer is expired

  if (gameTimer.isExpired())
  {
    changeMode(TIME_UP);
  }



}

void jackpotWinLoop ()
{
  if (isThrower == false)
  {
    setColor(YELLOW);
    //jackpot = false;
  }
  if (buttonSingleClicked())
  {
    changeMode(SELECT_TEAMS);
  }
}

void timeUpLoop ()
{
  setColor(RED);
  if (buttonSingleClicked())
  {
    changeMode(SELECT_TEAMS);
  }
}





/*
   pass this a game mode to switch to
*/
void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors
  if (gameMode == SELECT_TEAMS) //what the timer should be in each game state
  {
  }
  else if (gameMode == JACKPOT_SELECT)
  {
  }
  else if (gameMode == GAME)
  {
    gameTimer.set(GAME_DURATION); // start a game timer
  }
  else if (gameMode == TIME_UP)
  {
    gameTimer.set(0); //end just to be sure
  }
  else if (gameMode == JACKPOT_WIN)
  {
    //timer should never expire
    gameTimer.never(); // set the game timer to never expire
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

byte getThrowerStatus(byte data) {
  return ((data >> 4) & 1);//returns bit B
}

byte getGameMode(byte data) {
  return (data & 3);//returns bits E and F
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);//returns bits C and D
}

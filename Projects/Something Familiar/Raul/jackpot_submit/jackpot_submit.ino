//game mode switch
enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;

enum gameModes {MODE1, MODE2, MODE3, JACKPOT_WIN, TIME_UP};//these modes will simply be different colors
byte gameMode = MODE1;//the default mode when the game begins

// team select
Color teamColors[3] = {BLUE, CYAN, ORANGE};
byte color;
bool displayWhite = true;

//jackpot selection
bool jackpot = false;
byte pointValue;

//gameplay
Timer gameTimer;
#define GAME_DURATION 120000
bool isThrower = false;
bool isSpinning = true;
int receivedFive = -1;
int jackpotFacePosition = 0;
int twoPointFacePosition = 0;
int onePointFacePosition = 0;



void setup() {

  randomize();
  pointValue = random(1);

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
    case JACKPOT_WIN:
      jackpotWinLoop();
      break;
    case TIME_UP:
      timeUpLoop();
      break;
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData = (signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);
}

/*
   Mode 1-team select
*/
void mode1Loop() {
//change mode
  if (buttonDoubleClicked()) {
    changeMode(MODE2);
  }

  // change back to a blank slate if accidentaly clicked
  if (buttonMultiClicked())
  {
    displayWhite = true;
    isThrower = false;
  }

//blank slate
  if (displayWhite == true)
  {
    setColor(WHITE);
  }

  //set up a team blink
  if (buttonSingleClicked())
  {
    displayWhite = false;
    color = (color + 1) % 3;
    setColor(teamColors[color]);
    isThrower = true;
  }
  if (isThrower == true)
  {
    setValueSentOnAllFaces(5);
  }

}

/*
   Mode 2
*/
void mode2Loop() {

  setColor(OFF);

//set the jackpot blink
  if (buttonSingleClicked())
  {
    jackpot = true;
  }
  if (jackpot == true)
  {
    setColorOnFace(YELLOW, (millis() / 250) % 6);
  }

//change mode to game play
  if (buttonDoubleClicked()) {
    changeMode(MODE3);
  }


}

/*
   Mode 3- actual gameplay
*/
void mode3Loop() {
  //start the timer

  gameTimer.set(GAME_DURATION);

  //implement thrower status

  if (isThrower == true)
  {
    setValueSentOnAllFaces(5);
    setColor(teamColors[color]);
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
    //if its not a thrower, reset its received 5 value before checking
    receivedFive = -1;

    FOREACH_FACE(f)
    {
      if (getLastValueReceivedOnFace(f) == 5 && !isValueReceivedOnFaceExpired(f)) //if I have a neighbor and it send 5
      {
        //if i have received a value of 5 on any face, log that in received 5
        receivedFive = f;
      }
    }

    //if a received 5 value has been logged, stop the spinning of that blink
    if (receivedFive >= 0)
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
      if (receivedFive == jackpotFacePosition && jackpot == true)
      {
        //send to jackpot win
        gameTimer.never();
        changeMode(JACKPOT_WIN);
      }
      //send to whichever type of blink its connected to (2 pt or 1 pt)
      if (receivedFive == twoPointFacePosition && pointValue == 1)
      {
        setColor(GREEN);
        gameTimer.never();
      }
      if (receivedFive == onePointFacePosition && pointValue == 0)
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

void jackpotWinLoop (void)
{
  setColor(YELLOW);
  if (buttonSingleClicked())
  {
    changeMode(MODE1);
  }
}

void timeUpLoop (void)
{
  setColor(RED);
  if (buttonSingleClicked())
  {
    changeMode(MODE1);
  }
}





/*
   pass this a game mode to switch to
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

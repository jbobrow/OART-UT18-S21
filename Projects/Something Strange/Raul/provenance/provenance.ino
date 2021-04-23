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

enum gameModes {GAME, FAIL, SUCCESS};//The modes of the game
byte gameMode = GAME;//the default mode when the game begins

//game stuff

//________
//fade timers
Timer dimTimer;
#define DIM_TIME 8500
//_______
//shape recognition
int neighborCount = 0;
//_______
//color changing
Color cycleColors[4] = {OFF, MAGENTA, BLUE, YELLOW};
int cycleCount = 1;
bool increment = false;
bool sendColor = false;
int colorCheck = 3;


//_____

void setup() {

  //set the dim timer and randomize the color to be set to
  dimTimer.set(DIM_TIME);
  randomize();
  colorCheck = random(3) + 1;
  
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
  // share both signalState (i.e. when to change) and the game mode and the color 
  byte sendData = (getColor() << 4) + (signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);
}

/*
   Mode 1
*/
void gameLoop() {

  //get the remaining time on the dim timer
  int fadeTime = dimTimer.getRemaining();

  //mape in order to use it
  byte brightness = map(fadeTime, 0, DIM_TIME, 0, 255);

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

  //if I have 5 neighbors and we aren't changing anything and we are the right color, I win
  if (neighborCount == 5 && signalState == INERT && colorCheck == cycleCount)
  {
    changeMode(SUCCESS);
  }

  //switch the color if im alone

  if (isAlone())
  {
    sendColor = true;
    if (increment == true)
    {
      //increment in the cycle count and adjust it so it can fit inside the array
      cycleCount++;
      cycleCount = (cycleCount % 3) + 1;
      increment = false;
    }
  }
  else
  {
    //if I'm in a cluster, I gain the ability to increment
    increment = true;
  }

  //display logic

  //if the blink is completely dark, the player loses
  if (dimTimer.isExpired() == true)
  {
    changeMode(FAIL);
  }
  else
  {
    //display and dim the color that I am 
    setColor(dim(cycleColors[cycleCount], brightness));
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

  //if the game I am in is not GAME and I received orders to change to that, re-randomize and reset the dimming
  if(mode == GAME && gameMode != GAME)
  {
    dimTimer.set(DIM_TIME);
    colorCheck = random(3) + 1;
  }
  
  gameMode = mode;  // change my own mode
  signalState = GO; // signal my neighbors

  //if i'm in success or fail, don't change the dim timer
  if (gameMode == FAIL) {
    dimTimer.never();
  }
  else if (gameMode == SUCCESS) {
    dimTimer.never();
  }
}


void changeColor (byte color)
{
  //set my color to the cycleCount and tell my neighbors to go
  cycleCount = color;
  signalState = GO; // signal my neighbors
  //set my color
  setColor(cycleColors[cycleCount]);
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

        //get the color of my neighbor and change myself to that color
        byte neighborColor = getColorState(getLastValueReceivedOnFace(f));
        if (neighborColor > 0)
        {
          changeColor(neighborColor);
        }
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

byte getColor(void)
{
  //if I can send a color or I am in go and I am not in a cluster, send go. Regardless return my cyclecount to be used
  if (sendColor || signalState == GO)
  {
    if (!isAlone())
    {
      sendColor = false;
      signalState = GO;
    }
    return cycleCount;
  }
  else
  {
    return 0;
  }
}


byte getGameMode(byte data) {
  return (data & 3);//returns bits E and F
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);//returns bits C and D
}

//grab my color
byte getColorState(byte data) {
  return ((data >> 4) & 3);//returns bits A and B
}

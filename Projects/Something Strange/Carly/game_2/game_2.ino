enum signalStates {READY, PATTERN, GAME};
byte signalState = READY;

enum gameModes {MODE1, MODE2, MODE3, MODE4};// game modes determining the diffuculty of the game (also includes the ready/starting mode)
byte gameMode = MODE1;//the default mode when the game begins

Timer gameTimer;
#define GAME_DURATION 6000 // 6 seconds

Timer starTimer;
int waitDuration = (1+ random(5) ) * 1000; // gives a value of 1000, 2000, ...., or 6000

bool star = false;

//pattern stuff
byte pattern1[4] = {1, 2, 3, 4};

byte id;

byte step = 0;

// index
byte index = 0;

//pattern timer
Timer patternTimer;
Timer stepTimer;


// sparkle colors
Color autoColors[5] = {OFF, makeColorRGB(255, 0, 128), makeColorRGB(255, 255, 0), makeColorRGB(0, 128, 255), WHITE};

void setup() {
  randomize();
  id = 1 + random(6);
}

void loop() {

  // The following listens for and updates game state across all Blinks
  switch (signalState) {
    case READY:
      readyLoop();
      break;
    case PATTERN:
      patternLoop();
      break;
    case GAME:
      gameLoop();
      break;
  }

  // game state loops
  switch (gameMode) {
    case MODE1: // ready phase
      mode1Loop();
      break;
    case MODE2: //hardest diffuculty game mode
      mode2Loop();
      break;
    case MODE3: // medium diffuculty game mode
      mode3Loop();
      break;
    case MODE4: //easy diffuculty game mode
      mode4Loop();
      break;
  }

  // communicate with neighbors
  // share both signalState (i.e. when to change) and the game mode
  byte sendData = (signalState << 2) + (gameMode);
  setValueSentOnAllFaces(sendData);
}


void mode1Loop() {

  // press to start
  if (buttonPressed()) {
    changeMode(MODE2);  // change game mode on all Blinks
    patternTimer.set(6000);
    step = 0;
    stepTimer.set(1000);

  }

  setColor(autoColors[random(3) + 1]);  //creates sparkle lights
}


void mode2Loop() {

  // map the time remaining from 0 - 6000 milliseconds to the value 0-6
  byte timeRemaining = map(gameTimer.getRemaining(), 0, GAME_DURATION, 0, 6);

  // display how much time is left in the game (0-6 LEDs on)
//  FOREACH_FACE(f) {
//    if (starTimer.set) {
//      setColor(WHITE);
//      star = true;
//    }
//  
//    
//  }

/*  if (gameTimer.isExpired()) {
    changeMode(MODE3);
  } */

  if(stepTimer.isExpired()) {
    step++;
    stepTimer.set(1000);

    if(step > 5) {
      step = 0;
    }
  }

  
 if(pattern1[step] == id ) {
    // light up when my ID is active
   setColor(WHITE); 
   star = true;
  }
  
  else {
    setColor(OFF);
    star = false;
  } 
  
  if ( (star = false) && (buttonPressed()) ) { //if guessed wrong, switch to to medium game mode
    setColor(RED);
    changeMode(MODE3);
  }

  // check and dump button pressed during gameplay
  //buttonPressed();
}


void mode3Loop() {
  
  if ( (star = false) && (buttonPressed()) ) { //if guessed wrong, switch to easist game mode
    changeMode(MODE4);
    setColor(RED);
  }
  
  if ( (star = true) && (buttonPressed()) )  { //if guessed right, switch back to hardest game mode
    changeMode(MODE2);
  }

  setColor(BLUE);
}

void mode4Loop() {
  if ( (star = true) && (buttonPressed()) )  { //if guessed right, switch back to medium game mode
    changeMode(MODE3);
  }
}

/*
   pass this a game mode to switch to
*/
void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = PATTERN; // signal my neighbors

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
void readyLoop() {
 

  //listen for neighbors in GO
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == PATTERN) {//a neighbor saying GO!
        byte neighborGameMode = getGameMode(getLastValueReceivedOnFace(f));
        changeMode(neighborGameMode);
      }
    }
  }
}

/*
   If all of my neighbors are in GO or RESOLVE, then I can RESOLVE
*/
void patternLoop() {
  signalState = GAME;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not heard the GO news
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == READY) {//This neighbor doesn't know it's GO time. Stay in GO
        signalState = PATTERN;
      }
    }
  }
}

/*
   This loop returns me to inert once everyone around me has RESOLVED
   Now receive the game mode
*/
void gameLoop() {
  signalState = READY;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not moved to RESOLVE
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == PATTERN) {//This neighbor isn't in RESOLVE. Stay in RESOLVE
        signalState = GAME;
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

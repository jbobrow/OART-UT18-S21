enum signalStates {READY, PATTERN, GAME};
byte signalState = READY;

enum gameModes {MODE1, MODE2, MODE3, MODE4};// game modes determining the diffuculty of the game (also includes the ready/starting mode)
byte gameMode = MODE1;//default ready mode when the game begins

bool star = false;

//patterns for each game mode
byte pattern1[4] = {1, 2, 3, 4};
byte pattern2[4] = {4, 3, 2, 1};
byte pattern3[4] = {3, 2, 4, 1};

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
  randomize(); // randomizes which blinks are "stars" 
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
    stepTimer.set(500);

  }

  setColor(autoColors[random(3) + 1]);  //creates sparkle lights
}


void mode2Loop() {

  if(stepTimer.isExpired()) {
    step++;
    stepTimer.set(700);

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
  
  if ( (star == false) && (buttonPressed()) ) { //if guessed wrong, switch to to medium game mode
    setColor(RED);
    changeMode(MODE3);
  }

}


void mode3Loop() {
  
  if(stepTimer.isExpired()) {
    step++;
    stepTimer.set(1000);

    if(step > 5) {
      step = 0;
    }
  }
  
  if(pattern2[step] == id ) {
    // light up when my ID is active
   setColor(WHITE); 
   star = true;
  }
  
  else {
    setColor(OFF);
    star = false;
  } 
  
  if ( (star == false) && (buttonPressed()) ) { //if guessed wrong, switch to easist game mode
    changeMode(MODE4);
    setColor(RED);
  }
  
  if ( (star == true) && (buttonPressed()) )  { //if guessed right, switch back to hardest game mode
    changeMode(MODE2);
  }

  
}

void mode4Loop() {
  
  if(stepTimer.isExpired()) {
    step++;
    stepTimer.set(2000);

    if(step > 5) {
      step = 0;
    }
  }
  
  if(pattern3[step] == id ) {
    // light up when my ID is active
   setColor(WHITE); 
   star = true;
  }
  
  else {
    setColor(OFF);
    star = false;
  } 
  
  if ( (star == true) && (buttonPressed()) )  { //if guessed right, switch back to medium game mode
    changeMode(MODE3);
  }
}


void changeMode( byte mode ) {
  gameMode = mode;  // change my own mode
  signalState = PATTERN; // signal my neighbors

}



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


void patternLoop() {
  signalState = GAME;

  //look for neighbors who have not heard the GO news
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getSignalState(getLastValueReceivedOnFace(f)) == READY) {//This neighbor doesn't know it's GO time. Stay in GO
        signalState = PATTERN;
      }
    }
  }
}


void gameLoop() {
  signalState = READY;
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

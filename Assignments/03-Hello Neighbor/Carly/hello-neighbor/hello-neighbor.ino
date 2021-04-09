
// game states
enum Mode {
  READY,
  COUNTDOWN,
  PATTERN,
  GAME,
  END
};


byte mode = READY;

// pattern info
byte pattern1[6] = {1, 2, 3, 4, 5, 6};
byte pattern2[6] = {2, 2, 3, 1, 3, 5};
byte pattern3[6] = {5, 3, 1, 1, 4, 5};

byte id;

byte step = 0;

// index
byte index = 0;


bool flashOn = false;


// sparkle colors
Color autoColors[5] = {OFF, makeColorRGB(255, 0, 128), makeColorRGB(255, 255, 0), makeColorRGB(0, 128, 255), WHITE};

// countdown timer
Timer countdownTimer;
#define COUNT_TIME 6000 // 6 sec

//pattern timer
Timer patternTimer;
Timer stepTimer;


void setup() {
  
  randomize();
  id = 1 + random(5);

  setColor(RED);

}

void loop() {

   switch ( mode ) {
    
    case READY: // ready stage before game starts
      readyLoop();
      break;

    case COUNTDOWN: // countdown to game start
      countdownLoop();
      break;
      
    case PATTERN: // random pattern of flashes displayed before user input
      patternLoop();
      break;

    case GAME:
      gameLoop(); // where user inputs the pattern
      break;

    case END:
      endLoop(); // where user goes when they incorrectly input the pattern
      break;
  }

  setValueSentOnAllFaces(mode);
}

void readyLoop() {
  
  setColor(autoColors[random(3) + 1]);  //creates sparkle lights 
  
  if( buttonSingleClicked() ) {  //if single clicked will move into countdown timer
    //start countdown
    mode = COUNTDOWN;
    countdownTimer.set( COUNT_TIME );
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getLastValueReceivedOnFace(f) == COUNTDOWN) {//a neighbor saying GO!
    mode = COUNTDOWN;
    countdownTimer.set( COUNT_TIME );      
      }
    }
  }

}

void countdownLoop() {
  
  setColorOnFace(WHITE, (millis() / 1000) % 6); //creates countdown timer 6 sec
  
  if(countdownTimer.isExpired()){  // if countdown done, moves into pattern display
    mode = PATTERN;
    setValueSentOnAllFaces(1);
    patternTimer.set(6000);
    step = 0;
    stepTimer.set(1000);
  }

 /* bool same = true; 
  while(same) //these whiles make sure that the same color is not picked twice
  {
    second = random(2);
      if(second!=first)
      {
        same = false;
      }
  }
   same = true; 
  while(same)
  {
    third = random(2);
      if(third!=first && third != second) 
      {
        same = false;
      }
  } */
}

void patternLoop() {
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
  }
  
  else {
    setColor(OFF);
  } 

  if(patternTimer.isExpired()) {
    mode = END;
  }

//shows randomly assigned id # on each blink
 /* FOREACH_FACE(f) {
    if(id > f) {
      setColorOnFace(YELLOW, f);
    }
  }
  */
}

void gameLoop() {
  
}

void endLoop() {
  
  setColor(RED);

  if(buttonDoubleClicked()) { // double click will restart the game to countdown
    mode = COUNTDOWN;
  }
  else if(buttonLongPressed()) { // long press will take you back to the start
    mode = READY;
  }
}

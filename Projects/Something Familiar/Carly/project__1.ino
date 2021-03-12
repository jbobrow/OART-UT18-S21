// game states
enum Mode {
  SETUP,
  READY,
  COUNTDOWN,
  PATTERN,
  GAME,
  END
};

byte mode = SETUP;

//ids
byte ids[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
#define UNASSIGNED 10 
byte myIdIndex = UNASSIGNED; 

// colors
Color autoColors[5] = {OFF, makeColorRGB(255, 0, 128), makeColorRGB(255, 255, 0), makeColorRGB(0, 128, 255), WHITE};
Color PINK = makeColorRGB(252, 73, 121);

// countdown timer
Timer countdownTimer;
#define COUNT_TIME 6000 // 6 sec

//pattern timer
Timer patternTimer;
#define PATTERN_TIME 8000 // 8 sec

// game timer
Timer gameTimer;
#define GAME_TIME 10000 // 10 sec



void setup() {  
  
   
}

void loop() {

   switch ( mode ) {

    case SETUP: // setup stage where ids are assigned
      setupLoop();
      break;
    
    case READY: // ready stage before game starts
      readyLoop();
      break;

    case COUNTDOWN: // countdown to game start
      countdownLoop();
      break;
      
    case PATTERN: // where color corresponding to the blink's id are shown for a period of time
      patternLoop();
      break;

    case GAME:
      gameLoop(); // where user seperates the blinks into thier assigned pairs
      break;

    case END:
      endLoop(); // where user goes when they incorrectly guess a pair
      break;
  }

  setValueSentOnAllFaces(mode); 
}

void setupLoop() {
  
  // listen for a button click
  if (buttonSingleClicked() ) {
    myIdIndex = 0;  // I'm first in line
    setValueSentOnAllFaces(1);  // tell my neighbors
  }

  // listen to neighbors for assignment
  if (myIdIndex == UNASSIGNED) {  // only assign if not yet assigned
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) { // If I have a neighbor here do the following
        if (getLastValueReceivedOnFace(f) == 1) {
          myIdIndex = 1;
          setValueSentOnAllFaces(2);
        }
        else if (getLastValueReceivedOnFace(f) == 2) {
          myIdIndex = 2;
          setValueSentOnAllFaces(3);
        }
        else if (getLastValueReceivedOnFace(f) == 3) {
          myIdIndex = 3;
          setValueSentOnAllFaces(4);
        }
        else if (getLastValueReceivedOnFace(f) == 4) {
          myIdIndex = 4;
          setValueSentOnAllFaces(5);
        }
        else if (getLastValueReceivedOnFace(f) == 5) {
          myIdIndex = 5;
          setValueSentOnAllFaces(6);
        }
        else if (getLastValueReceivedOnFace(f) == 6) {
          myIdIndex = 6;
          setValueSentOnAllFaces(7);
        }
        else if (getLastValueReceivedOnFace(f) == 7) {
          myIdIndex = 7;
          setValueSentOnAllFaces(8);
        }
        else if (getLastValueReceivedOnFace(f) == 8) {
          myIdIndex = 8;
          setValueSentOnAllFaces(9);
        }
        else if (getLastValueReceivedOnFace(f) == 9) {
          myIdIndex = 9;
          setValueSentOnAllFaces(10);
        }
      }
    }
  }


  //display colors

  if (myIdIndex != UNASSIGNED) {
    // display my assigned color

    if (ids[myIdIndex] == 1) {
      setColor(MAGENTA);
    } else if (ids[myIdIndex] == 2) {
      setColor(BLUE);
    } else if (ids[myIdIndex] == 3) {
      setColor(GREEN);
    } else if (ids[myIdIndex] == 4) {
      setColor(YELLOW);
    } else if (ids[myIdIndex] == 5) {
      setColor(PINK);
    }
  }
  else {
    // display a rainbow when not yet assigned
    setColorOnFace(RED, 0);
    setColorOnFace(ORANGE, 1);
    setColorOnFace(YELLOW, 2);
    setColorOnFace(GREEN, 3);
    setColorOnFace(BLUE, 4);
    setColorOnFace(MAGENTA, 5);
  }
 
  if (buttonDoubleClicked() ) { // if double clicked will move into ready 
    mode = READY;
  }

  FOREACH_FACE(f) { // moves all blinks to ready loop
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getLastValueReceivedOnFace(f) == READY) {
         mode = READY;
         countdownTimer.set( COUNT_TIME );      
      }
    }
  }
}


void readyLoop() {
  
  setColor(autoColors[random(3) + 1]);  //creates sparkle lights 
  
  if( buttonSingleClicked() ) {  //if single clicked will move into countdown timer
    //start countdown
    mode = COUNTDOWN;
    countdownTimer.set( COUNT_TIME );
  }

  FOREACH_FACE(f) { //moves all blinks to countdown loop
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getLastValueReceivedOnFace(f) == COUNTDOWN) {
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
    patternTimer.set(PATTERN_TIME);
  }

 
}

void patternLoop() {

  if (myIdIndex != UNASSIGNED) { //displays color based on what id the blink has
    
    if (ids[myIdIndex] == 1) { // display my assigned color
      setColor(MAGENTA);
    } else if (ids[myIdIndex] == 2) {
      setColor(BLUE);
    } else if (ids[myIdIndex] == 3) {
      setColor(GREEN);
    } else if (ids[myIdIndex] == 4) {
      setColor(YELLOW);
    } else if (ids[myIdIndex] == 5) {
      setColor(PINK);
    }
  }

  if (patternTimer.isExpired()) { // pattern timer expired, move to game
    mode = GAME;
    gameTimer.set(GAME_TIME);
  }
  
}

void gameLoop() {
  setColor(WHITE);

  // if the time is up, check to see if my neighbor is my pair
//  if (gameTimer.isExpired()) { 
//    FOREACH_FACE(f) {
//      if (!isValueReceivedOnFaceExpired(f)) { // If I have a neighbor here do the following
//        if ((getLastValueReceivedOnFace(f) == ) { // if my id is the same as neighbor id, sucess & restart game
//          mode = READY; 
//        } else { // if my id is not the same as neighbor id, end game
//          mode = END; 
//        }
//      }
//    }
//  }
  
}

void endLoop() {
  
  setColor(RED);

  if(buttonDoubleClicked()) { // double click will restart the game to countdown
    mode = READY;
  }
}

/*
   Game State Demonstration
   by Jonathan Bobrow
   02.17.2021

   Tutorial to show how to create game states
   Change game states
   Use timers, enums
*/

// states for game play
enum Mode {
  INSERT_COIN,  // = 0
  GAMEPLAY,     // = 1
  END,          // = 2
  NUM_MODES     // = 3
};

byte mode;

Timer gameTimer;
#define GAME_DURATION 10000 // 10 seconds duration

// create a game character
byte distance = 0;


void setup() {
  // put your setup code here, to run once:
  mode = INSERT_COIN;
}

void loop() {
  // put your main code here, to run repeatedly:

  switch ( mode ) {

    case INSERT_COIN:
      idleLoop();
      break;

    case GAMEPLAY:
      gameLoop();
      break;

    case END:
      endLoop();
      break;
  }
}

/*
   Idle Loop
   Do this when the game is not actively being played
*/
void idleLoop() {
  // do stuff related to the insert coin mode

  // handle user interaction
  if ( buttonDoubleClicked() ) {
    // start the game if double clicked
    mode = GAMEPLAY;
    distance = 0;
    
    // set the duration for the game timer
    gameTimer.set( GAME_DURATION );
  }
  
  setColor(YELLOW);
}

/*
   Gameplay Loop
   Handle all in game logic here, not end game though
*/
void gameLoop() {
  // do stuff related to the gameplay mode
  setColor(GREEN);
  
  // if the user presses the button, move 1 distance
  if(buttonPressed()) {
    distance = distance + 1;
  }

  // show our current position on one of the 6 faces
  byte facePosition = distance % 6; // 0 - 5
  
  setColorOnFace( WHITE, facePosition);

  // if the game time is up, let's go to the end mode
  if (gameTimer.isExpired()) {
    mode = END;
  }

  // ignore double clicks
  if(buttonDoubleClicked()) {
    // do nothing
  }
}

/*
   End Loop
   Show the end result of the game
*/
void endLoop() {
  // do stuff related to the end mode
  // handle button presses
  if(buttonDoubleClicked()) {
    mode = INSERT_COIN;
  }
  
  setColor(OFF);

  // display how far the player got
  // for every 10 distance, light up a face
  for( byte i=0; i<6; i++) {  // i will loop through 0-5
   
   if(distance / 10 > i) {  // every 10 steps we light up another face
    setColorOnFace(RED, i); 
   }
  }
}

enum Mode { 
  START,
  GAME,
  END,
  NUM_MODES
};

  byte mode;


Timer gameTimer;
#define GAME_DURATION 5000

 byte distance = 0;


void setup() {
  // put your setup code here, to run once:
mode = START;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  switch ( mode ) {
  case START:
    idleLoop();
    break;
  
  case GAME:
    gameLoop();
    break;
  
  case END:
  endLoop();
   break;
  }
}

void idleLoop() {

  if ( buttonSingleClicked() ) {
    mode = GAME;
    distance = random(6);

    gameTimer.set( GAME_DURATION );
  }
  setColor(YELLOW);
}

void gameLoop() {
  setColor(GREEN);

  if(buttonPressed()) {
    distance =;
  }


  byte facePosition = distance % 6;

  setColorOnFace( WHITE, facePosition);

  if(gameTimer.isExpired()) {
    mode = END;
  }

  if(buttonDoubleClicked()) {
  }
}

void endLoop() {
  if(buttonDoubleClicked()) {
    mode = START;
  }

  setColor(OFF);

  for( byte i=0; i<6; i++) {

    if(distance / 10 > i) {
      setColorOnFace (RED, i);
    }
  }
}
   
    
    

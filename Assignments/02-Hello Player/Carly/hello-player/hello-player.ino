enum Mode {
  READY,
  COUNTDOWN,
  GAMEPLAY,
  END
};

// pattern info
#define MAX_PATTERN_LENGTH 4
word pattern[MAX_PATTERN_LENGTH] = {2, 1, 1, 2};

byte index = 0;

Timer roundTimer;
#define ROUND_TIME 10000 
bool activeRound = false;

bool flashOn = false;

byte mode = READY;

Color autoColors[5] = {OFF, makeColorRGB(255, 0, 128), makeColorRGB(255, 255, 0), makeColorRGB(0, 128, 255), WHITE};

//timers
Timer countdownTimer;
#define COUNT_TIME 6000 // 6 sec

bool buttonFlash = false;
Timer flashTimer;
#define FLASH_TIME 1000

bool hitButton = false;
Timer hitTimer;
byte hit = 0;


void setup() {
  // put your setup code here, to run once:
  randomize();
//  createPatternOfLength(4);

}

void loop() {
  // put your main code here, to run repeatedly:
  switch ( mode ) {
    
    case READY:
      readyLoop();
      break;

    case COUNTDOWN:
      countdownLoop();
      break;
      
    case GAMEPLAY:
      gameLoop();
      gameDisplayLoop();
      break;

    case END:
      endLoop();
      break;
  }
}

void readyLoop() {
  setColor(autoColors[random(3) + 1]);
  
  if( buttonSingleClicked() ) {
    //start countdown
    mode = COUNTDOWN;
    countdownTimer.set( COUNT_TIME );
  }
}

void countdownLoop() {
  setColorOnFace(YELLOW, (millis() / 1000) % 6);
  if(countdownTimer.isExpired()){
    mode = GAMEPLAY;
    roundTimer.set(ROUND_TIME);
  }
}

void gameLoop() {

  for (byte i = 0; i < 3; i++) {
    if (flashTimer.isExpired()) {

    flashOn = !flashOn;

    if (pattern[index] == 0) {
      flashOn = true;
    }

    flashTimer.set(FLASH_TIME);

    index++;
  }
    setColor(makeColorRGB(0, 0, 255 * flashOn));
  }
  
  if(buttonPressed()) {
    if (hitButton) { 
      hitButton = false;
      flashOn = true;
    }
   }

  if(hitButton && roundTimer.isExpired()) { 
         mode = END;
      } 
  
}



void gameDisplayLoop() {
  if (hitButton) {
    setColor(BLUE);
  }
  
}

void endLoop() {
  setColor(RED);

  if(buttonDoubleClicked()) {
    mode = COUNTDOWN;
  }
}

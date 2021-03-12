#define COOLDOWN_DURATION 3000

enum signalStates {INERT, RESET, RESOLVE};
byte signalState = INERT;
enum blinkModes {FRISBEE, GOAL, VICTORY};
byte blinkMode = GOAL;
byte team=0;
byte score=0;

Timer scoreCooldown;
// Timer animTimer;

byte sendData= (team << 4)+(signalState << 2)+(blinkMode);

Color teamColor[4]={RED, BLUE, ORANGE, CYAN};

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  switch (signalState) {
    case INERT:
      inertLoop();
      break;
    case RESET:
      resetLoop();
      break;
    case RESOLVE:
      resolveLoop();
      break;
  }
  
  if(blinkMode == GOAL) {
    team = 3;
    setColor(teamColor[team]);
  }
  else if(blinkMode == FRISBEE) {
    teamSet();
    frisbeeDisplay();
  }
  else{
    victoryDisplay();
  }
  
  byte sendData= (team << 4)+(signalState << 2)+(blinkMode);
  setValueSentOnAllFaces(sendData);
}

void inertLoop() {
  if (buttonMultiClicked()) {
    byte clicks = buttonClickCount();
    if(clicks == 3){
      signalState = RESET;
    }
  }

  // set desired blink as frisbee
  if(buttonLongPressed()){
    if(isAlone()){
      blinkMode = FRISBEE;
      team=0;
      teamSet();
    }
  }

  // team select
  if(blinkMode == FRISBEE){
    if(buttonDoubleClicked()){
      if(isAlone()){
        team++;
        if(team == 3){
          team=0;
        }
        teamSet();
      }
    }
  }
  
  // did I score
  if(blinkMode == FRISBEE){
    int facesConnected = 0;
    FOREACH_FACE(f){
      if(!isValueReceivedOnFaceExpired(f)){
        if(getBlinkMode(getLastValueReceivedOnFace(f)) == GOAL){
          facesConnected++;
        }
      }
    }
    if (scoreCooldown.isExpired()) {
      if (facesConnected == 3) {
        score + 2;
        scoreCooldown.set(COOLDOWN_DURATION);
      }
      else if (facesConnected == 1) {
        score++;
        scoreCooldown.set(COOLDOWN_DURATION);
      }
    }

    if (score == 6) {
      blinkMode = VICTORY;
    }
    else {
      blinkMode = FRISBEE;
    }
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getSignalState(getLastValueReceivedOnFace(f)) == RESET) {
        signalState = RESET;
      }
    }
  }
}

// reset the game
void resetLoop() {
  signalState = RESOLVE;

  blinkMode = GOAL;
  score = 0;
  team = 0;

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getSignalState(getLastValueReceivedOnFace(f)) == INERT) {
        signalState = RESET;
      }
    }
  }
}

void resolveLoop() {
  signalState = INERT;

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getSignalState(getLastValueReceivedOnFace(f)) == RESET) {
        signalState = RESOLVE;
      }
    }
  }
}

void frisbeeDisplay(){
  FOREACH_FACE(f) {
    if (f < score) {
      setColorOnFace(WHITE, f);
    }
    else {
      setColorOnFace(teamColor[team], f);
    }
  }
}

void victoryDisplay() {
  if (blinkMode == VICTORY) {
    setColor(GREEN);
    /*
    if (animTimer.isExpired()) {
      setColor(GREEN);
      animTimer.set(1500);
    }
    else {
      setColor(OFF);
      //animTimer.set(750);
    }
    */
  }
}

void teamSet(){
  setColor(teamColor[team]);
}

//team at [A], signalState at [C][D], blinkMode at [E][F]
byte getBlinkMode(byte data) {
  return (data & 3);
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);
}

byte getTeam(byte data) {
  return (data >> 4);
}

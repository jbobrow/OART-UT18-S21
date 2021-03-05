enum signalStates {
  INERT,
  RESET,
  RESOLVE
};
byte signalState = INERT;

enum modes {
  FRISBEE,
  GOAL,
  VICTORY
};
byte mode = GOAL;

byte team = 0;
byte score = 0;
//byte scoreOnFace[6] = {0,0,0,0,0,0};
Color teamColor[4] = {RED, BLUE, WHITE, YELLOW};

byte sendData = (team << 4) + (signalState << 2) + (mode);

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
      resolveLoop;
      break;
  }
  if (mode=GOAL) {
    setColor(WHITE);
  }
  else if (mode = FRISBEE) {
    frisbeeDisplay();
  }
  else {
    //display the victory animation
    victoryAnimation();
  }

  byte sendData = (team << 4) + (signalState << 2) + (mode);
  setValueSentOnAllFaces(sendData);
}

void inertLoop() {
  if (buttonMultiClicked()){
    byte clicks = buttonClickCount();
    if (clicks == 3) {
      signalState = RESET;
    }
  }
  //select which blink you want as your frisbee
  if (buttonLongPressed()) {
    if(isAlone()) {
      mode = FRISBEE;
      team = 0;
      frisbeeDisplay();
    }
  }

  //team select
  if (mode == FRISBEE) {
    if (buttonDoubleClicked()) {
      if (isAlone()) {
        team++;
        if (team == 2) { team = 0; }
        frisbeeDisplay();
      }
    }
  }

  //checking if a goal was made
  if (mode == FRISBEE) {
    byte facesConnected = 0;
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {
        if (getBlinkMode(getLastValueReceivedOnFace(f))==GOAL) {
          facesConnected++;
        }
      }
    }
    if (facesConnected == 3) {
      score++;
    }
    if (score == 6) {
      mode = VICTORY;
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

void resetLoop() {
  signalState = RESOLVE;

  mode = GOAL;
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
      setColorOnFace(CYAN, f);
    }
    else {
      setColorOnFace(teamColor[team], f);
    }
  }
}

void victoryAnimation() {
  if (mode == VICTORY){
    setColor(GREEN);
  }
}

byte getBlinkMode(byte data) {
  return (data & 3);
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);
}

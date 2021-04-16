enum signalStates {INERT, RESET, RESOLVE};
byte signalState = INERT;
enum blinkModes {CONNECTED_PATH, UNCONNECTED_PATH, SUCCESS, FAIL};
byte blinkMode = UNCONNECTED_PATH;
byte connectedFace1;
byte connectedFace2;
byte currColor = 0;
bool activePath = false;

byte sendData = (currColor << 4)+(signalState << 2)+(blinkMode);
byte sendSuccess = (currColor << 4)+(signalState << 2)+(SUCCESS);

Color colorOptions[4]={GREEN, BLUE, ORANGE, CYAN};

//byte onFace = (millis() / 500) % 6;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
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
  if (!activePath) {
    setValueSentOnAllFaces(sendData);
  }
}

void inertLoop() {
  if (buttonMultiClicked()) {
    byte clicks = buttonClickCount();
    if(clicks == 3){
      signalState = RESET;
    }
  }
  // CODE FOR MAKING THE RING OF BLINKS
  if (blinkMode == UNCONNECTED_PATH) {
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)){
        connectedFace1 = f;
      }
    }

    // the next blink in the loop should be placed 2 faces away (either clockwise or counterclockwise)
    // try 2 faces away clockwise
    connectedFace2 = connectedFace1 + 2;
    if (connectedFace2 > 5) {
      connectedFace2 = connectedFace2 - 6;
    }
    // check if 2 faces away clockwise is not connected
    if (isValueReceivedOnFaceExpired(connectedFace2)) {
      // must then be 2 faces away counterxfc clockwise
      connectedFace2 = connectedFace1 - 2;
      if (connectedFace2 < 0) {
        connectedFace2 = connectedFace2 + 6;
      }
    }
  }

  // visualize whether or not the game is set up correctly
  // all blinks in a valid loop will be off
  if (isAlone()) {
    blinkMode = UNCONNECTED_PATH;
    //setColor(WHITE);
  }
  else if (!isValueReceivedOnFaceExpired(connectedFace1) && !isValueReceivedOnFaceExpired(connectedFace2) && blinkMode != FAIL) {
    blinkMode = CONNECTED_PATH;
    //setColor(OFF);
  }
  else if (!isValueReceivedOnFaceExpired(connectedFace1) && isValueReceivedOnFaceExpired(connectedFace2)) {
    blinkMode = UNCONNECTED_PATH;
    //setColor(MAGENTA);
  }
  else if (isValueReceivedOnFaceExpired(connectedFace1) && !isValueReceivedOnFaceExpired(connectedFace2)) {
    blinkMode = UNCONNECTED_PATH;
    //setColor(MAGENTA);
  }

  // BEGIN GAMEPLAY
  // this is to start the game initially
  if (blinkMode == CONNECTED_PATH && buttonDoubleClicked()) {
    activePath = true;
  }
  // if we did not start off the game, but we received a success signal from a blink
  if(getBlinkMode(getLastValueReceivedOnFace(connectedFace1)) == SUCCESS){
    activePath = true;
  }
  else if(getBlinkMode(getLastValueReceivedOnFace(connectedFace2)) == SUCCESS){
    activePath = true;
  }

  if (activePath) {
    activePathLoop();
  }

  FOREACH_FACE(f){
    if(!isValueReceivedOnFaceExpired(f)){
      if(getBlinkMode(getLastValueReceivedOnFace(f)) == FAIL){
        blinkMode = FAIL;
      }
    }
  }
  if (blinkMode == FAIL) {
    failLoop();
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getSignalState(getLastValueReceivedOnFace(f)) == RESET) {
        signalState = RESET;
      }
    }
  }
}

void activePathLoop(){
  byte onFace = (millis() / 250) % 6;
  setColor(dim(colorOptions[currColor], 75));
  setColorOnFace(colorOptions[currColor], onFace);

  if (buttonPressed()) {
    // successfully move to the next blink in the loop
    if (onFace == connectedFace1 || onFace == connectedFace2){
      setColor(colorOptions[currColor]);
      activePath = false; // make sure we don't branch into the active path code again
      currColor += 1; // we want to display the next color if we make a full loop
      // tell the next blink in the loop our movement was successful
      if (onFace == connectedFace1) {
        setValueSentOnFace(sendSuccess, connectedFace1);
      }
      else if (onFace == connectedFace2) {
        setValueSentOnFace(sendSuccess, connectedFace2);
      }
    }
    // button pressed but does not lead to movement to the next blink in loop
    else {
      blinkMode = FAIL;
    }
  }
}

void failLoop() {
  // display failure animation
  setColor(dim(RED, 127));
  setColorOnFace(RED, (millis() / 500) % 6);
  
  // ENTER RESET SIGNAL STATE
  if (buttonMultiClicked()) {
    byte clicks = buttonClickCount();
    if(clicks == 3){
      signalState = RESET;
    }
  }
}

void resetLoop() {
  signalState = RESOLVE;

  blinkMode = UNCONNECTED_PATH;
  currColor = 0;
  setColor(OFF);
  activePath = false;

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

byte getBlinkMode(byte data) {
  return (data & 3);
}

byte getSignalState(byte data) {
  return ((data >> 2) & 3);
}
byte getColor(byte data) {
  return (data >> 4);
}

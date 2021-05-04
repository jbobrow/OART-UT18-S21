//fully functioning epidemic! Andrew Tebbe 4/22/21
enum State {
  SUSCEPTIBLE,   // 0
  IMMUNE,    // 1
  SICK,
  RECOVERED,
  DEAD
};

Timer stepTimer;
Timer sickTime;
#define STEP_DURATION 500 //timme before code enacted

byte myState;

void setup() {
  randomize();
  initialize();
}

void loop() {

  if (buttonSingleClicked() )
  {
    sickTime.set(2000); //STARTS THE TIMER
    myState = SICK; // START  THE SICK
    sickLoop();
  }
  
  if (buttonDoubleClicked()) {
  	initialize();
  }

  if (stepTimer.isExpired()) {
    stepTimer.set(STEP_DURATION);

    // DO STUFF AS A suspect
    if (myState == SICK) {
      sickLoop();  
    }
    else if (myState == SUSCEPTIBLE) {
      susceptibleLoop();
    }
    // DO STUFF AS immune
    else if (myState == IMMUNE) {
      immuneLoop();
    }
    else if (myState == RECOVERED) {
      recoveredLoop();
    }
    else if (myState == DEAD) {
      deadLoop();
    }
  }

  // Share our state
  setValueSentOnAllFaces(myState);

  // display myState
  switch (myState) {
    case SUSCEPTIBLE: setColor(YELLOW); break;
    case IMMUNE: setColor(GREEN); break;
    case SICK: setColor (ORANGE); break;
    case RECOVERED: setColor (CYAN); break;
    case DEAD: setColor (RED); break;
  }
}

void initialize() {

  if (random(100) < 50) {
    myState = SUSCEPTIBLE; //NUMBER OF SUSCEPTIBLE 80%
  }
  else {
    myState = IMMUNE;
  } //IMMUNE 20%
}

void sickLoop() {
  if (sickTime.isExpired())  //WANT TO SELECT RECOVERED OR DEAD AFTER  1 SECOND. NOT WORKING!!
  {
    if (random(100) < 80) {
      myState = RECOVERED; //RECOVERED 80%
    }
    if (random(100) > 80) { // DEAD 20%
      myState = DEAD;
    }
  }
}
void susceptibleLoop() {

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == SICK && random(100) < 70)  //CHANCE YOU GET SICK
      {
        myState = SICK;
        sickTime.set(2000); //DURATION YOU ARE SICK FOR
      }
      else if (neighborVal == SICK && random(100) > 70) //CHANCE YOU BECOME IMMUNE
      {
        myState = RECOVERED;
      }
    }
  }
}

void immuneLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == SICK && random(100) < 1)  //CHANCE YOU GET SICK VERY LOW BECAUSE IMMUNE
      {
        myState = SICK;
        sickTime.set(2000); //DURATION YOU ARE SICK FOR
      }
    }
  }
}

void deadLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == SICK)  
      {
        //DO NOTHING
      }
    }
  }
}

void recoveredLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == SICK )  
      {
        //DO NOTHING
      }
    }
  }
}

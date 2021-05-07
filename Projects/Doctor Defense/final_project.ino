
enum State {
  START,
  SUSCEPTIBLE,   // 1
  IMMUNE,    // 2
  SICK,
  RECOVERED,
  DEAD,
  DOCTOR,
  GOAL,
  END
};

Timer stepTimer;
Timer sickTime;
Timer resetTimer;
#define STEP_DURATION 2000 //time before code enacted

// end animation timer
Timer endTimer;
#define END_TIME 2000 //time before end animation


byte myState = START;

void setup() {
  randomize();
}

void loop() {

  if (buttonSingleClicked () )
  {
    sickTime.set(5000); //STARTS THE TIMER
    myState = SICK; // START  THE SICK
    sickLoop();
  }
  if ( buttonDoubleClicked()) { //declare a immune
    myState = GOAL;
  }

  if (buttonMultiClicked() ) //declare a doctor
  {
    myState = DOCTOR;
  }

  if (buttonLongPressed() )
  {
    setValueSentOnAllFaces(1);
    myState = SUSCEPTIBLE;

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
    else if (myState == GOAL) {
      goalLoop();
    }
    else if (myState == END) {
      endLoop();
    }
    else if (myState == DOCTOR) {
      doctorLoop();
    }
  }

  // Share our state
  setValueSentOnAllFaces(myState);

  // display myState
  switch (myState) {
    case START: startLoop(); break;
    case SUSCEPTIBLE: setColor (makeColorRGB (255, 157, 0)); break; //orange
    case IMMUNE: setColorOnFace (makeColorRGB (238, 255, 0), (millis() / 200) % 6); break; // yellow
    case SICK: setColorOnFace(makeColorRGB(72, 255, 0), (millis() / 200) % 6); break; // green
    case RECOVERED: setColorOnFace (makeColorRGB (3, 230, 255), (millis() / 200) % 6); break; // cyan
    case DEAD: setColorOnFace (makeColorRGB (255, 38, 0), (millis() / 200) % 6); break; // red
    case DOCTOR: setColor (makeColorRGB (177, 3, 252)); break; // purple
    case GOAL: setColor(WHITE); break;
    case END: endLoop(); break;

  }
}

void startLoop() {
  myState = SUSCEPTIBLE; //NUMBER OF SUSCEPTIBLE 80%

}

void doctorLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == END) {
        myState = END;
        endTimer.set(END_TIME);
      }
    }
  }


}

void sickLoop() {
  if (sickTime.isExpired())  //WANT TO SELECT RECOVERED OR DEAD AFTER  1 SECOND. NOT WORKING!!
  {
    if (random(100) < 60) {
      myState = RECOVERED; //RECOVERED 80%
    }
    if (random(100) > 60) { // DEAD 20%
      myState = DEAD;
    }
  }
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == DOCTOR)  //CHANCE YOU GET SICK
      {
        myState = IMMUNE;
        //sickTime.set(3000); //DURATION YOU ARE SICK FOR
      }
      if (neighborVal == END) {
        myState = END;
        endTimer.set(END_TIME);
      }
    }
  }
}
void susceptibleLoop() {
  if (sickTime.isExpired()) { //delay before becoming sick
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {

        byte neighborVal = getLastValueReceivedOnFace(f);

        if (neighborVal == SICK && random(100) < 70)  //CHANCE YOU GET SICK
        {
          myState = SICK;
          sickTime.set(3000); //DURATION YOU ARE SICK FOR
        }
        else if (neighborVal == SICK && random(100) > 70) //CHANCE YOU BECOME IMMUNE
        {
          myState = RECOVERED;
        }
        if (neighborVal == END) {
          myState = END;
          endTimer.set(END_TIME);
        }
      }
    }
  }


}


void immuneLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == SICK && random(100) < 5)  //CHANCE YOU GET SICK VERY LOW BECAUSE IMMUNE
      {
        myState = SICK;
        sickTime.set(2000); //DURATION YOU ARE SICK FOR
      }
      if (neighborVal == END) {
        myState = END;
        endTimer.set(END_TIME);
      }
    }
  }
}

void deadLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == END) {
        myState = END;
        endTimer.set(END_TIME);
      }
    }
  }


  FOREACH_FACE(f) { //SEARCHES FOR LONG PRESS TO BE RESET
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == 1) {
        myState = SUSCEPTIBLE;

      }
    }
  }


}

void goalLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == SICK)  //CHANCE YOU GET SICK
      {
        myState = END;
        endTimer.set(END_TIME);
        #define STEP_DURATION 500
      }
    }
  }
}

void recoveredLoop() {
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == END) {
        myState = END;
        endTimer.set(END_TIME);
        resetTimer.set(2000);
      }
    }
  }

  FOREACH_FACE(f) { //SEARCHES FOR LONG PRESS TO BE RESET
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == 1) {
        myState = SUSCEPTIBLE;

      }
    }
  }
}

void endLoop() {
#define STEP_DURATION 500
  if (endTimer.isExpired()) {
    setColorOnFace(makeColorRGB (255, 38, 0), (millis() / 100) % 6);
  }
  FOREACH_FACE(f) { //SEARCHES FOR LONG PRESS TO BE RESET
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == 1) {
        myState = SUSCEPTIBLE;

      }
    }
  }
}

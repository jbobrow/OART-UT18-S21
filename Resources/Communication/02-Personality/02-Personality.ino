/*
   Let's make Blinks talk to each other
   Share our value
*/

enum Personality {
  EXTRAVERT,  // 0
  INTROVERT   // 1
};

byte myPersonality = EXTRAVERT;

byte neighbors[6];

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // react to user input
  if (buttonPressed()) {
    if (myPersonality == EXTRAVERT) {
      myPersonality = INTROVERT;
    }
    else if (myPersonality == INTROVERT) {
      myPersonality = EXTRAVERT;
    }
  }

  // Listen to my neighbors
  FOREACH_FACE(f) {
    // loop through all of them
    if (!isValueReceivedOnFaceExpired(f)) {
      // if my neighbor is present, get their value

      byte neighborVal = getLastValueReceivedOnFace(f); // listen

      neighbors[f] = neighborVal; // save for later
    }
    else {
      neighbors[f] = 2;
    }
  }

  // Tell everyone who are you :)
  setValueSentOnAllFaces(myPersonality);

  // display my personality
  switch (myPersonality) {
    case EXTRAVERT:
      setColor(makeColorHSB(20, 255, 255));
      break;
    case INTROVERT:
      setColor(makeColorHSB(200, 255, 255));
      break;
  }

  // display our neighbors
  FOREACH_FACE(f) {
    switch (neighbors[f]) {
      case EXTRAVERT:
        setColorOnFace(makeColorHSB(20, 255, 255),f);
        break;
      case INTROVERT:
        setColorOnFace(makeColorHSB(200, 255, 255),f);
        break;
    }
  }
}

#define STEP_DURATION 500

enum State { EMPTY , RABBIT, FOX };
//byte statesOnBlink[6];
byte myState = EMPTY;

byte foxDatagram[] { 'F' };
byte rabbitDatagram[] { 'R' };

byte validFaces[6]; // we need an array to store valid faces for our predators/prey to move to
int numValidFaces = 0; // will be used fill the array and as the upper bound for random()

Timer stepTimer;

bool debug = false;

void setup() {
  randomize();
  
  byte seeding = random(100);
  if (0 <= seeding < 33) {
    myState = RABBIT;
  }
  else if (33 <= seeding < 66) {
    myState = FOX;
  }
  else {
    myState = EMPTY;
  }
}

int memcmp(const void *s1, const void *s2, unsigned n) {
  if (n != 0) {
    const unsigned char *p1 = s1, *p2 = s2;

    do {
      if (*p1++ != *p2++)
        return (*--p1 - *--p2);
    } while (--n != 0);
  }
  return (0);
}

void loop() {
  if (buttonSingleClicked()) {
    myState = RABBIT;
  }
  else if (buttonDoubleClicked()) {
    myState = FOX;
  }

  if (stepTimer.isExpired()) {
    stepTimer.set(STEP_DURATION);

    // Empty
    if (myState == EMPTY) {
      emptyLoop();
    }

    // Rabbit
    else if (myState == RABBIT) {
      numValidFaces = 0; // reset our valid faces
      FOREACH_FACE(f) {
        if (!isValueReceivedOnFaceExpired(f)) {
          // rabbits can only jump to empty spots
          if (getLastValueReceivedOnFace(f) == EMPTY) {
            validFaces[numValidFaces] = f;
            numValidFaces++;
          }
        }
      }
      preyLoop();
    }

    // Fox
    else if (myState == FOX) {
      numValidFaces = 0; // reset our valid faces
      FOREACH_FACE(f) {
        if (!isValueReceivedOnFaceExpired(f)) {
          // foxes can move anywhere except where ther is already a fox
          if (getLastValueReceivedOnFace(f) != FOX) {
            validFaces[numValidFaces] = f;
            numValidFaces++;
          }
        }
      }
      predatorLoop();
    }
  }

  // Share our state
  setValueSentOnAllFaces(myState);

  // display myState
  switch (myState) {
    case EMPTY:
      setColor(OFF);
      break;
    case RABBIT: 
      if (!debug){
        setColor(WHITE);
        break;
      }
      else {
        for (int i = 0; i < numValidFaces; i++) {
          setColorOnFace(WHITE, validFaces[i]);
        }
      }
    case FOX:
      if (!debug){
        setColor(ORANGE);
        break;
      }
      else {
        for (int i = 0; i < numValidFaces; i++) {
          setColorOnFace(ORANGE, validFaces[i]);
        }
      }
  }
}

void emptyLoop() {
  // Do nothing? The only way my state can change is if it is prompted to by a rabbit or fox.
  // if i received a datagram on one of my faces
  // check if fox or rabbit and switch states accordingly
  // not really sure how to handle if a rabbit and fox try to move to the same blink; fox should probably win out
  bool gotFox = false; // use this to track if a fox moved to the blink at any point, but was overwritten by a rabbit since

  FOREACH_FACE(f) {
    if (isDatagramReadyOnFace(f)) {
      const byte *datagramPayload = getDatagramOnFace(f);
      if (getDatagramLengthOnFace(f)==sizeof( foxDatagram )  &&  !memcmp( datagramPayload , foxDatagram , sizeof( foxDatagram ))) {
        gotFox = true;
        myState = FOX;
      }
      else if (getDatagramLengthOnFace(f)==sizeof( rabbitDatagram )  &&  !memcmp( datagramPayload , rabbitDatagram , sizeof( rabbitDatagram ))) {
        myState = RABBIT;
      }
      markDatagramReadOnFace(f);
    }
  }

  if (myState == RABBIT && gotFox) { // our state was overwritten by a rabbit, but we had previously seen a fox
    myState = FOX;
  }

  // FOREST FIRE EXAMPLE:
  // 5% chance to become a rabbit
  //if (random(100) <= 5) {
  //  myState = RABBIT;
  //}
}

void preyLoop() {
  // as a rabbit, I should move to a neighboring empty slot on each step update
  // rabbits have a 5% chance of leaving behind offspring in the spot they previously occupied
  // otherwise they leave behind an empty spot

  // from ellibable faces, randomly select where to send datagram
  // percentage chance to leave behind offspring
  if (numValidFaces > 0) { // we have somewhere to move
    byte moveTo = validFaces[random(numValidFaces)];
    sendDatagramOnFace( &rabbitDatagram , sizeof( rabbitDatagram ) , moveTo );
    if (random(100) < 25) { // percentage chance to leave behind offspring
      myState = RABBIT;
    }
    else { // space is now empty otherwise
      myState = EMPTY;
    }
  }
  else {
    myState = EMPTY; // we had nowhere to go, just die for now I guess?
  }
  // if I receive a fox datagram, i should turn into a fox
  FOREACH_FACE(f) {
    if (isDatagramReadyOnFace(f)) {
      const byte *datagramPayload = getDatagramOnFace(f);
      if (getDatagramLengthOnFace(f)==sizeof( foxDatagram )  &&  !memcmp( datagramPayload , foxDatagram , sizeof( foxDatagram ))) {
        myState = FOX;
      }
      markDatagramReadOnFace(f);
    }
  }

/*
  if (random(100) < 3) {
    myState = FIRE;
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {

      byte neighborVal = getLastValueReceivedOnFace(f);

      if (neighborVal == FIRE) {
        myState = FIRE;
      }
    }
  }
*/
}


void predatorLoop() {
  // as a fox, I should move to a neighboring spot if at least 1 of my neighbors is a rabbit
  // I should leave behind offspring in the spot I previously occupied
  // I have a 15% chance to turn into an empty spot

  // from ellibable faces, randomly select where to send datagram
  if (numValidFaces > 0) { // we have somewhere to move
    byte moveTo = validFaces[random(numValidFaces)];
    sendDatagramOnFace( &foxDatagram , sizeof( foxDatagram ) , moveTo );
    if (!isValueReceivedOnFaceExpired(moveTo)) {
      if (getLastValueReceivedOnFace(moveTo) == RABBIT) {
        myState = FOX;
      }
      else {
        myState = EMPTY;
      }
    }
  }
  // I have a percentage chance to die
  if (random(100) < 40) {
    myState = EMPTY;
  }
  // FOREST FIRE EXAMPLE:
  // myState = EMPTY;
}
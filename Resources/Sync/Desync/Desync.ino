/*
   Allow a ring of Blinks to create a continuous spectrum
   based on this Desync algorithm published by Harvard
   https://dash.harvard.edu/bitstream/handle/1/25680331/tr-18-06.pdf?sequence=1&isAllowed=y
*/

#define PERIOD 3000 // 3 second period
#define ALPHA 95  // 95% jump

Timer n1_timer;
Timer n2_timer;

byte n1_face = 6;
byte n2_face = 6;

Timer fireTimer;
bool justFired = false;

byte fireValue = 1;

byte neighborVals[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  n1_timer.never();
  n2_timer.never();
  n1_face = 6;
  n2_face = 6;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonPressed()) {
    fireTimer.set(0);
    n1_timer.never();
    n2_timer.never();
    n1_face = 6;
    n2_face = 6;
    // reset neighbor vals
    FOREACH_FACE(f) {
      neighborVals[f] = 0;
    }
  }

  // if the fire timer expires
  if (fireTimer.isExpired()) {
    // share fire message
    fireValue = 1 + (fireValue % 2);  // toggles between 1 and 2
    setValueSentOnAllFaces(fireValue);
    fireTimer.set(PERIOD);
    justFired = true;
  }

  // keep neigbor timers up to speed
  if (n1_timer.isExpired()) {
    n1_timer.set(PERIOD);
  }
  if (n2_timer.isExpired()) {
    n2_timer.set(PERIOD);
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte neighborVal = getLastValueReceivedOnFace(f);
      if ( (neighborVal != 0) && (neighborVal != neighborVals[f]) ) {
        // change happened here
        neighborFiredOnFace(f);

        if ( justFired ) {
          justFired = false;
          // update to our goal
          chaseGoal();
        }

      }
      neighborVals[f] = neighborVal;
    }
  }

  // display the states
  // display a hue for where I am in my timer
  byte hue = map(fireTimer.getRemaining(), 0, PERIOD, 0, 255);
  setColor(makeColorHSB(hue, 255, 255));

  // if I fired set my color to white for 100ms
  if (fireTimer.getRemaining() > (PERIOD - 100)) {
    setColor(WHITE);
  }

}

void neighborFiredOnFace(byte f) {

  // assign to neighbor if not yet assigned
  if (n1_face == 6) {
    n1_face = f;
  }
  else if (n2_face == 6 && f != n1_face) {
    n2_face = f;
  }

  // update neighbor timers to reflect their internal timers
  if ( f == n1_face ) {
    n1_timer.set(PERIOD);
  }
  else if ( f == n2_face ) {
    n2_timer.set(PERIOD);
  }
}

void chaseGoal() {
  if (n1_face != 6 && n2_face != 6) {
    // both have been initialized, let's move towards being between them
    uint16_t range;
    uint16_t goal;

    // n2 just fired
    if ( (n2_timer.getRemaining() - n1_timer.getRemaining()) > 0 ) {
      range = n2_timer.getRemaining() - n1_timer.getRemaining();
      if ( range < (PERIOD / 2) ) {
        goal = n1_timer.getRemaining() + range / 2;
        if ( goal > PERIOD ) {
          goal = goal - PERIOD;
        }
      }
      else {
        goal = n2_timer.getRemaining() + (PERIOD - range) / 2;
        if ( goal > PERIOD ) {
          goal = goal - PERIOD;
        }
      }

    }
    // n1 just fired
    else {
      range = n1_timer.getRemaining() - n2_timer.getRemaining();
      if ( range < (PERIOD / 2) ) {
        goal = n2_timer.getRemaining() + range / 2;
        if ( goal > PERIOD ) {
          goal = goal - PERIOD;
        }
      }
      else {
        goal = n1_timer.getRemaining() + (PERIOD - range) / 2;
        if ( goal > PERIOD ) {
          goal = goal - PERIOD;
        }
      }
    }

    // let's go between...
    // set my timer ALPHA(95%) of the way towards the goal
    uint16_t goalTime = ( fireTimer.getRemaining() * (100 - ALPHA) + (goal * ALPHA ) ) / 100;
    fireTimer.set(goalTime);
  }
}

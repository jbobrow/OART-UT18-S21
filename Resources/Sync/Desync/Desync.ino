/*
   Allow a ring of Blinks to create a continuous spectrum
   based on this Desync algorithm published by Harvard
   https://dash.harvard.edu/bitstream/handle/1/25680331/tr-18-06.pdf?sequence=1&isAllowed=y
*/

#define PERIOD 3000 // 3 second period
#define ALPHA 80  // 95% jump

struct Neighbor {
  Timer timer;
  byte face;
};

Neighbor n1;
Neighbor n2;

Timer fireTimer;
bool justFired = false;

byte fireValue = 1;

byte neighborVals[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  n1.timer.never();
  n2.timer.never();
  n1.face = 6;
  n2.face = 6;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonPressed()) {
    fireTimer.set(0);
    n1.timer.never();
    n2.timer.never();
    n1.face = 6;
    n2.face = 6;
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
  if (n1.timer.isExpired()) {
    n1.timer.set(PERIOD);
  }
  if (n2.timer.isExpired()) {
    n2.timer.set(PERIOD);
  }

  FOREACH_FACE(f) {
    if (isValueReceivedOnFaceExpired(f)) continue;
    byte neighborVal = getLastValueReceivedOnFace(f);
    if ( neighborVal != 0 && neighborVal != neighborVals[f] ) {
      // change happened here
      neighborFiredOnFace(f);
    }
    neighborVals[f] = neighborVal;
  }

  if ( justFired ) {
    // update to our goal
    chaseGoal();
    justFired = false;
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
  if (n1.face == 6) {
    n1.face = f;
  }
  else if (n2.face == 6) {
    n2.face = f;
  }

  // update neighbor timers to reflect their internal timers
  if ( f == n1.face ) {
    n1.timer.set(PERIOD);
  }
  else if ( f == n2.face ) {
    n2.timer.set(PERIOD);
  }
}

void chaseGoal() {
  if (n1.face != 6 && n2.face != 6) {
    // both have been initialized, let's move towards being between them
    uint16_t range;
    
    if( n2.timer.getRemaining() - n1.timer.getRemaining() > 0 ) {
      range = n2.timer.getRemaining() - n1.timer.getRemaining();
    }
    else {
      range = n1.timer.getRemaining() - n2.timer.getRemaining();
    }

    if ( range < (PERIOD / 2) ) {
      // let's go between...
      // set my timer ALPHA(95%) of the way towards the goal
      uint16_t goalTime = ( fireTimer.getRemaining() * (100 - ALPHA) + (range / 2) * ALPHA ) / 100;
      fireTimer.set(goalTime);
    }
    else {
      // let's go between...
      // set my timer ALPHA(95%) of the way towards the goal
      uint16_t goalTime = ( fireTimer.getRemaining() * (100 - ALPHA) + ((PERIOD - range) / 2) * ALPHA ) / 100;
      fireTimer.set(goalTime);
    }
  }

}

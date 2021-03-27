/*
   Allow a ring of Blinks to create a continuous spectrum
   based on this Desync algorithm published by Harvard
   https://dash.harvard.edu/bitstream/handle/1/25680331/tr-18-06.pdf?sequence=1&isAllowed=y
*/

#define PERIOD 3000 // 3 second period
#define ALPHA 95  // 95% jump

bool just_fired = false;
uint32_t my_fire = 0;
uint32_t next_fire = 0;
uint32_t prev_fire = 0;
uint32_t last_fire = 0;

Timer fireTimer;
Timer slotStartTimer;
Timer slotEndTimer;

byte fireValue = 1;
byte last_fire_neighbor = 0;

byte neighborVals[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonPressed()) {
    fireTimer.set(0);
  }

  // if the fire timer expires
  if (fireTimer.isExpired()) {
    // share fire message
    fireValue = 1 + (fireValue % 2);  // toggles between 1 and 2
    setValueSentOnAllFaces(fireValue);
    just_fired = true;
    my_fire = millis();
    prev_fire = last_fire;
    fireTimer.set(PERIOD);
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

  // display the states
  // if I fired set my color to white for 300ms
  if (fireTimer.getRemaining() > (PERIOD - 300)) {
    setColor(WHITE);
  }
  else {
    setColor(OFF);
  }
  if ((millis() - last_fire) < 300) {
    setColorOnFace(GREEN, last_fire_neighbor);
  }
}

void neighborFiredOnFace(byte f) {

  last_fire = millis();
  last_fire_neighbor = f;

  uint32_t time_since_my_fire = millis() - my_fire;
  uint32_t time_since_prev_fire = millis() - prev_fire;

  // if a neighbor fires
  uint32_t slot_start = PERIOD + (time_since_prev_fire + time_since_my_fire ) / 2;
  uint32_t slot_end = PERIOD + (last_fire + time_since_my_fire) / 2;
  uint32_t goal_time = PERIOD + (1 - ALPHA) * time_since_my_fire + ALPHA * time_since_prev_fire;

  // update my timer to the goal
  uint32_t now = PERIOD - fireTimer.getRemaining(); // time passed
  fireTimer.set(goal_time - now);
  //  slotStartTimer.set(slot_start - now);
  //  slotEndTimer.set(slot_end - now);
}

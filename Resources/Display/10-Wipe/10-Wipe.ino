/*  --------------------
    10-Wipe
    by Jonathan Bobrow
    Move38, Inc. 2021

    Note: animation used in Astro
    --------------------
*/

#define NUM_COLORS 4
#define ROTATION_STEPS 7
#define STEP_DURATION  50

Color colors[] = {BLUE, YELLOW, MAGENTA, GREEN};

Timer wipeTimer;

byte val = 0;

void setup() { 
}

void loop() {
  
  displayWipe();

}


void   displayWipe() {

      if (wipeTimer.isExpired()) { // add a new face for the next color

      // every 80 ms
      wipeTimer.set(STEP_DURATION);

      //
      val = (val + 1) % ( FACE_COUNT * ROTATION_STEPS * NUM_COLORS);

      byte index = (val / ROTATION_STEPS) % NUM_COLORS;
      setColorOnFace(colors[index], val % FACE_COUNT);
    }
}

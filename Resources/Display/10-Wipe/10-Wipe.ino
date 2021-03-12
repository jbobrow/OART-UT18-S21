/*  --------------------
    10-Wipe
    by Jonathan Bobrow
    Move38, Inc. 2021

    Note: animation used in Astro
    --------------------
*/

#define NUM_COLORS 4
#define ROTATION_STEPS 7  // use 7 so that it offsets each rotation
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

      // Use a value other than 6 for Rotation Steps to have the starting point
      // of the rotation change each time around. In this case, we use 7
      val = (val + 1) % ( FACE_COUNT * ROTATION_STEPS * NUM_COLORS);

      byte index = (val / ROTATION_STEPS) % NUM_COLORS;
      setColorOnFace(colors[index], val % FACE_COUNT);
    }
}

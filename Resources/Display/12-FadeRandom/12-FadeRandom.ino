/*  --------------------
    12-Fade Random
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/
byte faceBri[6];

#define DECREMENT 5
#define FADE_INTERVAL 20

byte index = 0;
byte prev_index = 0;

Timer fadeTimer;

void setup() {
  // put your setup code here, to run once:
  randomize();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (buttonPressed()) {
    // choose an index that is different from the previous index
    while (index == prev_index) {
      index = random(5);
    }

    faceBri[index] = 255;
    prev_index = index;
  }


  if (fadeTimer.isExpired()) {

    FOREACH_FACE(f) {
      if (faceBri[f] > DECREMENT) {
        faceBri[f] -= DECREMENT;
      }
    }
    fadeTimer.set(FADE_INTERVAL);
  }


  // display our bri
  FOREACH_FACE(f) {
    setColorOnFace(dim(WHITE, faceBri[f]), f);
  }
}

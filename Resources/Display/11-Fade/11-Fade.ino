/*  --------------------
    11-Fade
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

byte faceBri[6];

#define DECREMENT 5
#define FADE_INTERVAL 20

byte index = 0;

Timer fadeTimer;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  if (buttonPressed()) {
    faceBri[index] = 255;
    index++;

    if (index >= 6) {
      index = 0;
    }
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

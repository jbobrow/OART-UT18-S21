/*
 * 01_RecordPlayback
 * 
 * This sketch is exactly what it sounds like. For making dits and doots.
 * 
 * It records a pattern and plays it back. 
 * 
 * Can you guess what the boot up pattern is?!
 * 
 * Spoiler alert: https://gearside.com/custom-vibration-patterns-mobile-devices/
 * 
 * Happy Holidays E'rybody :)
 * 
 * --------------------
 * Sketch-A-Day
 * 12.15.2018
 * by Jonathan Bobrow
 * 
 * @madewithblinks
 * www.move38.com
 * --------------------
 */

#define TIMEOUT_DURATION 2000
word pattern[128] = {125,75,125,275,200,275,125,75,125,275,200,600,200,600};
byte playhead = 0;

enum Mode {
  RECORD,
  PLAYBACK
};

byte mode = PLAYBACK;

Timer recordTimer;
Timer playbackTimer;

bool isLightOn = false;

uint32_t timeOfPress = 0;
uint32_t timeOfRelease = 0;

void setup() {
}

void loop() {

  // don't let playback start while we are holding the button down
  if (buttonDown()) {
    recordTimer.set(TIMEOUT_DURATION);
  }

  // start recording if button is pressed
  // record the amount of time the button was up
  if (buttonPressed()) {

    timeOfPress = millis();
    isLightOn = true;

    if (mode == PLAYBACK) {
      mode = RECORD;
      clearPattern();
      resetPlayhead();
    } else {
      // start a new on period
      pattern[playhead] = (word)(timeOfPress - timeOfRelease);
      playhead++;
    }
  }

  // record the amount of time the button was down
  if (buttonReleased()) {

    timeOfRelease = millis();

    isLightOn = false;
    // record time on
    pattern[playhead] = (word)(timeOfRelease - timeOfPress);
    playhead++;
  }

  // check to see if our recording is done (no activity for TIMEOUT_DURATION)
  if ( mode == RECORD ) {
    if (recordTimer.isExpired()) {
      // enter playback
      pattern[playhead] = 1000; // insert 1 second of off between looping
      playhead++;
      mode = PLAYBACK;
    }
  }

  // playback the pattern that we entered
  if ( mode == PLAYBACK ) {
    if (playbackTimer.isExpired()) {

      isLightOn = !isLightOn;

      if (pattern[playhead] == 0) {
        resetPlayhead();
        isLightOn = true;
      }

      playbackTimer.set(pattern[playhead]);

      // advance playhead
      playhead++;
    }
  }

  // display the pattern in red when RECORD and green when PLAYBACK
  switch (mode) {
    case PLAYBACK: setColor(makeColorRGB(0, 255 * isLightOn, 0));   break;
    case RECORD:   setColor(makeColorRGB(255 * isLightOn, 0, 0));   break;
  }
}

void clearPattern() {
  // clear pattern
  for (byte i = 0; i < 255; i++) {
    pattern[i] = 0;
  }
}

void resetPlayhead() {
  // reset playhead
  playhead = 0;
}

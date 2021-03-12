/*
   02_RecordPlayback
   It records a pattern and plays it back.
   
   Can you guess what the boot up pattern is?!
   Spoiler alert: https://gearside.com/custom-vibration-patterns-mobile-devices/
   --------------------
*/

#define TIMEOUT_DURATION 2000
#define MAX_PATTERN_LENGTH 64
word pattern[MAX_PATTERN_LENGTH] = {125, 75, 125, 275, 200, 275, 125, 75, 125, 275, 200, 600, 200, 600};
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

  switch ( mode ) {

    case RECORD:
      recordLoop();
      break;

    case PLAYBACK:
      playLoop();
      break;
  }

}

/* RECORD
 * record a pattern of presses
 * stop the recording and begin playback 
 * if there are no presses for TIMEOUT_DURATION
 */
void recordLoop() {

  // don't let playback start while we are holding the button down
  if (buttonDown()) {
    recordTimer.set(TIMEOUT_DURATION);
  }

  if (buttonPressed()) {
    timeOfPress = millis();
    isLightOn = true;

    // start a new on period
    pattern[playhead] = (word)(timeOfPress - timeOfRelease);
    playhead++;
  }

  // record the amount of time the button was down
  if (buttonReleased()) {

    timeOfRelease = millis();

    isLightOn = false;
    // record time on
    pattern[playhead] = (word)(timeOfRelease - timeOfPress);
    playhead++;
  }

  if (recordTimer.isExpired()) {
    // enter playback
    pattern[playhead] = 3000; // insert 3 seconds of off between looping
    playhead++;
    mode = PLAYBACK;
  }

  // display the pattern in red when RECORD
  setColor(makeColorRGB(255 * isLightOn, 0, 0));
}

/* PLAYBACK
 * plays a pattern based on the durations stored in a pattern array
 * exit playback and start record if a button is pressed
 */
void playLoop() {

  if (buttonPressed()) {
    timeOfPress = millis();
    isLightOn = true;
    mode = RECORD;
    clearPattern();
    resetPlayhead();
  }

  // playback the pattern that we entered
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

  // display the pattern in green when PLAYBACK
  setColor(makeColorRGB(0, 255 * isLightOn, 0));
}

/*
 * Clear the array that stores the pattern
 */
void clearPattern() {
  for (byte i = 0; i < MAX_PATTERN_LENGTH; i++) {
    pattern[i] = 0;
  }
}

/*
 * Start the playhead from the beginning
 */
void resetPlayhead() {
  playhead = 0;
}

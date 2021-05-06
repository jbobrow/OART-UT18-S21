/*
   Spreading a message in the most precarious way possible

   Send the message we heard,
   then don't listen for a period of time

   This is how I used to propogate messages until Dan King created
   a much more stable solution. This sketch does do what you'd expect
   but it feels a bit more precarious than the validation that occurs with
   the Safe Communication tutorials

   by Jonathan Bobrow
   05.05.2021
*/

#define BROADCAST_DURATION 100
#define DEAF_DURATION 200
#define ANIMATION_DELAY 70

uint32_t timeOfSwitch = 0;

Color colors[] = {WHITE, CYAN, MAGENTA, BLUE, ORANGE, GREEN, RED};

byte myState = 1;
byte numStates = 6;

byte goToValue = 1;
Timer goToTimer;

void setup() {
  // nothing needed in setup
}

void loop() {

  // increment our state each time the button is pressed
  if (buttonPressed()) {
    setState(myState + 1);
  }


  // determine if we should listen to our neighbors
  if (timeOfSwitch < millis() - (BROADCAST_DURATION + DEAF_DURATION) ) {

    // we have broadcasted and ignored feedback, so we are good to listen again
    FOREACH_FACE(f) {

      // is there a Blink on this face?
      if (!isValueReceivedOnFaceExpired(f)) {

        // get their value
        byte neighbor = getLastValueReceivedOnFace(f);

        // is our neighbor different (since Blinks broadcast 0 by default, we don't want to respond to 0)
        if (neighbor != 0 && neighbor != myState) {
          // store neighbor value and change to it after delay
          goToTimer.set(ANIMATION_DELAY);
          goToValue = neighbor;

        }
      }
    }
  }

  if (goToTimer.isExpired()) {
    setState(goToValue);
    goToTimer.never();
  }

  // is the time passed our broadcast duration?
  if (timeOfSwitch < millis() - BROADCAST_DURATION ) {

    // return to sending 0
    setValueSentOnAllFaces(0);  // stop sending state
  }

  // display a color that matches our state
  setColor(colors[myState]);
}


/*
   setState
    - Set the state of the Blink to this value
    - Broadcast the value of the Blink on all faces
    - Set the time that we started broadcasting
*/
void setState(byte _state) {
  if (_state >= numStates) {
    myState = 1;
  }
  else {
    myState = _state;
  }

  timeOfSwitch = millis();

  setValueSentOnAllFaces(myState);  // start sending state
}

/*
   The following sketch passes a ball from Blink to Blink
*/

bool hasBall = false;

#define NOBODY 6
byte sendingTo = NOBODY;
byte receivedFrom = NOBODY;

enum PassStates {
  INERT,
  PASS,
  RECEIVED
};

Timer ballTimer;
#define HOLD_DURATION 300  // amount of time to hold onto the ball in milliseconds

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // create a ball if pressed
  if (buttonPressed()) {
    spawnBall();
  }

  // if we have the ball and we are done holding find a neighbor to pass to
  if (hasBall) {
    // if there are no neighbors to pass to after a period of time, lets drop the ball

    // choose one neighbor to pass to randomly
    if (sendingTo == NOBODY) {
      chooseRecipient();
    }

    if (ballTimer.isExpired()) {  // we've held the ball long enough, now what
      if (sendingTo != NOBODY) {  // we have someone chosen to send to
        if (!isValueReceivedOnFaceExpired(sendingTo)) { // they are still present
          if (getLastValueReceivedOnFace(sendingTo) == INERT) { // they are ready to receive
            setValueSentOnFace( PASS, sendingTo);
          }
          else if (getLastValueReceivedOnFace(sendingTo) == RECEIVED) { // they have received
            setValueSentOnFace( INERT, sendingTo);  // let them know we know they received
            hasBall = false;        // get rid of the ball
            sendingTo = NOBODY;     // forget who we sent to
            receivedFrom = NOBODY;  // forget who we received from
          }
        }
      }
      else {
        hasBall = false;            // drop the ball, we don't have anyone to give it to
        receivedFrom = NOBODY;      // forget who we received from
      }
    }

  }
  else {
    // if we don't have the ball,
    // listen to neighbors to see if they are trying to hand us the ball
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) { // look at present neighbors only
        byte neighbor = getLastValueReceivedOnFace(f);

        if (neighbor == PASS ) {
          // let's let them know we'll receive it
          receivedFrom = f;
          setValueSentOnFace( RECEIVED, receivedFrom);
        }
        else if ( neighbor == INERT ) {
          // if this is the neighbor we received from, confirm that we now have the ball
          if ( f == receivedFrom ) {
            spawnBall(); // accept the ball now that they will give it up
            setValueSentOnFace( INERT, receivedFrom); // no more need to let them know we received it
          }
        }

      }
    }
  }

  // display the ball if we have it
  if (hasBall) {
    setColor(WHITE);
  }
  else {
    setColor(OFF);
  }
}

/*
   A function to create a ball
   with a time that it will take
   to pass to a neighbor or expire
*/
void spawnBall() {
  hasBall = true;
  ballTimer.set(HOLD_DURATION);
}

/*
   A function to find a recipient
   if there is one suitable (i.e. not
   the Blink we just received the ball from)
*/
void chooseRecipient() {

  // all 6 neighbors start not present
  byte neighbors[6] = {false, false, false, false, false, false};

  bool hasPossibleRecipients = false;

  FOREACH_FACE(f) {

    // see which neighbors are present
    if (!isValueReceivedOnFaceExpired(f)) {
      if (f != receivedFrom) { // don't allow it to send back from where it came
        neighbors[f] = true;  // mark that this is a possible recipient (true = present)
        hasPossibleRecipients = true;
      }
    }
  }

  if (hasPossibleRecipients) {
    // choose one of the available neighbors to send to
    byte r = random(5);

    while (neighbors[r] == false) {
      r = random(5);
    }

    sendingTo = r;
  }
  else {
    return;
  }
}

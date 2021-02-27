/*
 * 
 */

// used to keep track of my neighbors state
byte neighbors[6];

// used to keep track of what I will display
byte myFaces[6];

byte myMessages[6];

enum pressStates{
  IDLE,
  PRESSED,
  RESOLVE
};

void setup() {
  // put your setup code here, to run once:

  FOREACH_FACE(f) {
    myFaces[f] = random(1); // set my own faces to random colors
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // when pressed, update my message to send that I pressed
  if(buttonPressed()) {
    
    FOREACH_FACE(f) {
      myMessages[f] = PRESSED;
    }
  }

  // when blink pressed tell all neighbors to flip their faces connect to me

  // listen to my neighbors. If on a face, my neighor says pressed. 
  // change my face to acknowledge
  FOREACH_FACE(f) {
    if( isValueReceivedOnFaceExpired(f) ) continue;

     byte neighborState = getLastValueReceivedOnFace(f);

     if( myMessages[f] == IDLE) {
        // check if my neighbor is pressed
        // if so, then I should update to RESOLVE
     }
     
     else if( myMessages[f] == PRESSED ) {
        // check if my neighbor is resolved
        // if so, then I should update to IDLE
     }

     else if (myMessages[f] == RESOLVE ) {
        // check if my neighbor is IDLE
        // if so, then I should update to IDLE
        // flip my state
     }
     
  }

  // send our values on all faces
  FOREACH_FACE(f) {
//    setValueSentOnFace(###, f);
  }

  // display my colors
  FOREACH_FACE(f) {
      switch (myFaces[f]) {
        case 0:
          setColorOnFace(BLUE,f);
          break;
        case 1:
          setColorOnFace(RED,f);
          break;
    }
  }
}

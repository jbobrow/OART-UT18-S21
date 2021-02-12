/*  --------------------
    05-Rotate
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

byte index = 0;

Timer incrementTimer;
#define STEP_DURATION 100

void setup() { 
}

void loop() {

  if( incrementTimer.isExpired() ) {

    index = ( index + 1 ) % 6;  // keep index cycling from 0 to 5
    
    incrementTimer.set( STEP_DURATION );  // set the timer to expire in STEP_DURATION milliseconds
  
  }
  
  setColor(OFF);  // clear the display from last loop
  
  setColorOnFace(WHITE, index); // display WHITE on our current index
  
}

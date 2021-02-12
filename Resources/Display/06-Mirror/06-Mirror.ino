/*  --------------------
    06-Mirror
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/


byte index = 0;

Timer incrementTimer;
#define STEP_DURATION 300

void setup() { 
}

void loop() {

  if( incrementTimer.isExpired() ) {

    index = ( index + 1 ) % 3;  // keep index cycling from 0 to 2
    
    incrementTimer.set( STEP_DURATION );  // set the timer to expire in STEP_DURATION milliseconds
  
  }

  setColor(OFF);  // clear the display from last loop

  setColorOnFace(WHITE, index);   // display WHITE on our current index   
  setColorOnFace(WHITE, 5-index); // display WHITE on the mirror of our current index   
    
}

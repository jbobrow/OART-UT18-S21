/*  --------------------
    07-Flip
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

void setup() { 
}

void loop() {

  byte deciseconds = millis() / 100; // milliseconds divided by 100 are deciseconds

  setColor(OFF);  // clear the display from last loop
  
  if( deciseconds % 2 == 0 ) {  // on even deciseconds flip one side
    setColorOnFace(YELLOW,0);
    setColorOnFace(YELLOW,1);
    setColorOnFace(YELLOW,2);
  }
  else {
    setColorOnFace(YELLOW,3);
    setColorOnFace(YELLOW,4);
    setColorOnFace(YELLOW,5);
  }
}

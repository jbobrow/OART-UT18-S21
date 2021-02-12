/*  --------------------
    01-Blink
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

void setup() { 
}

void loop() {

  byte seconds = millis() / 1000; // milliseconds divided by 1000 are seconds
  
  if( seconds % 2 == 0 ) {  // on even seconds display RED
    setColor(RED);
  }
  else {
    setColor(OFF);
  }
}

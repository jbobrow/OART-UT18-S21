/*
 * A function to count the number of neighbors present
 */
 
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  
  byte count = getNumNeighbors(); // store the count of neighbors

  // clear the display from last loop
  setColor(OFF);
  // display the count of neighbors
  // by lighting up a single LED for each neighbor counted
  FOREACH_FACE(f) {
    if( f < count ) { // if the LED id is less than the count
      setColorOnFace(WHITE, f); // light it up
    }
  }
}

/*
 * This function returns the number of neighbors I have
 */
byte getNumNeighbors() {
  
  byte numNeighbors = 0;
  
  FOREACH_FACE(f) { // look at each of my faces
    if(!isValueReceivedOnFaceExpired(f)) {  // a neighbor is present
      numNeighbors++; // add one to my count
    }
  }

  return numNeighbors;
}

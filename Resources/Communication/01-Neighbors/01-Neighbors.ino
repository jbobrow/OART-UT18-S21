/*
 * Let's make Blinks talk to each other
 * Count my Neighbors
 */

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  byte numNeighbors = 0;
  
  //for(int i=0; i<6; i++) {
  FOREACH_FACE(f) {
    if(!isValueReceivedOnFaceExpired(f)) {
      // add to our neighbor count
      numNeighbors++; // numNeighbors = numNeighbors + 1;
    }
  }

  // display count some way
  FOREACH_FACE(f) {
    
    if(f < numNeighbors) {
      setColorOnFace(WHITE,f);
    }
    else {
      setColorOnFace(OFF,f);
    }
  }
}

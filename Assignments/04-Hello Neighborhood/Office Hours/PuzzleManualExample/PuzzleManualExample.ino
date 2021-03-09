// Manual Puzzle101
byte faceValues[6];
Color colors[3] = {BLUE, YELLOW, RED};

void setup() {

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  setColor(OFF);
  
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      // when you see a neighbor
      if (getLastValueReceivedOnFace(f) == faceValues[f]) {
         // display the agreed upon color
         setColorOnFace(colors[faceValues[f]], f);
      }
      else {
        // try again
        faceValues[f] = random(3);
        
        // share the value we choose
        setValueSentOnFace(faceValues[f], f);
      }
    }
  }
  
  
}

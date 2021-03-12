byte faceValue = 0;
Timer timer;
 
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  
  // look at all of our faces (0-5)
  FOREACH_FACE(f) {

    if(didValueOnFaceChange(f)) {
      timer.set(2000);
    }    
  }  

  if(buttonPressed()) {
    faceValue++;  // because this is a byte, it will go from 0-255
  }

  setValueSentOnAllFaces(faceValue);  // broadcasting values between 0-63

  //display our face value
  if( faceValue < 16 ) {
    byte brightness = 16 * (1 + faceValue);
    setColor(dim(BLUE, brightness));
  }
  else if( faceValue >= 16 && faceValue < 32 ) {
    byte brightness = 16 * (1 + faceValue - 16);
    setColor(dim(GREEN, brightness));    
  }
  else if( faceValue >= 32 && faceValue < 48 ) {
    byte brightness = 16 * (1 + faceValue - 32);
    setColor(dim(RED, brightness));    
  }
  else if( faceValue >= 48 && faceValue < 64 ) {
    byte brightness = 16 * (1 + faceValue - 48);
    setColor(dim(WHITE, brightness));    
  }

  if(!timer.isExpired()) {
    setColor(MAGENTA);
  }
}

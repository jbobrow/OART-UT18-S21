//make blinks talk to each other

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  byte numNeighbors = 0;
  
    //for(int i = 0; i<6; i++) //goes through 0-5
    FOREACH_FACE(f){
      if(isValueReceivedOnFaceExpired(f)) //if expired = no neighbhor, if !expired = neighbhoor
      {
        numNeighbors++; 
      }
    }

    //display them
    FOREACH_FACE(f) 
    {
      if(f<numNeighbors)
      {
              setColorOnFace(BLUE, f); 
      } else{
        setColorOnFace(OFF, f);
      }
    }
}

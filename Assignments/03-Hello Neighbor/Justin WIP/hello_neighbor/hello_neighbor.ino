void setup() {
  randomize();
  // put your setup code here, to run once:
  setValueSentOnAllFaces(10);
  FOREACH_FACE(f) {
      byte curCol = random(2);
      if(curCol == 1){
        setColorOnFace(BLUE, f);
        setValueSentOnFace(20, f);
        
      }
      else if(curCol == 0){
        setColorOnFace(RED, f);
        setValueSentOnFace(30, f);
      }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(buttonPressed()){
    SwitchColors();
  }
}

void SwitchColors(){
  FOREACH_FACE(f) {
    if(isValueReceivedOnFaceExpired(0)){
      if(getLastValueReceivedOnFace(f) == 20){
        setColorOnFace(RED, f);
        setValueSentOnFace(30, f);
      }
      else if(getLastValueReceivedOnFace(f) == 30){
        setColorOnFace(BLUE, f);
        setValueSentOnFace(20, f);
      }
      else{
          byte curCol = random(2);
          if(curCol == 1){
            setColorOnFace(BLUE, f);
            setValueSentOnFace(20, f);
            
          }
          else if(curCol == 0){
            setColorOnFace(RED, f);
            setValueSentOnFace(30, f);
          }
    }
    }

    else if(!isValueReceivedOnFaceExpired(f)) {
        byte oldCol = getLastValueReceivedOnFace(f);
        
        if(oldCol == 1){
        setColor(BLUE);
        setValueSentOnFace(20, f);
        }
        
        else if(oldCol == 0){
          setColor(RED);
          setValueSentOnFace(30, f);
        }
    } 
}
}

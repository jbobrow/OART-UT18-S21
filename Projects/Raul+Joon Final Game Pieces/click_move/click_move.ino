bool isMoving = false;
bool transition = false;


void setup() {

}

void loop() {
  
  //general background
  if (isMoving == false)
  {
    //background color
    setColor(GREEN);
  }
  //select a blink we will be moving with
  if (buttonDoubleClicked())
  {
    //ability to toggle 
    isMoving = !isMoving; 
  }
  
  if (isMoving)
  {
    //rando color and value
    setColor(WHITE); 
    setValueSentOnAllFaces(3);
  }
  
  if (isMoving == false && buttonSingleClicked())
  {
    //single click a green blink to transition
    transition = true;
  }
  
  //wipe single clicks
  buttonSingleClicked();
  if(transition)
  {
    
   setValueSentOnAllFaces(4);
    //turn white and then close gate
    isMoving = true; 
    transition = false;
  }
  
  //if i was a a white blink and i am receiving a signal to transition, shut off
  FOREACH_FACE(f)
  {
   if (getLastValueReceivedOnFace(f) == 4 && isMoving == true)
    {
     isMoving = false; 
    }
  }

}

bool isBunny = false;
bool isFox = false;
bool transition = false;


void setup() {

}

void loop() {
  
  //general background
  if (isBunny == false || isFox == false)
  {
    //background color
    setColor(GREEN);
  }
  //select a blink we will be moving with
  if (buttonDoubleClicked())
  {
    //ability to toggle 
    isBunny = !isBunny; 
  }
  if (buttonMultiClicked())
  {
    //toggle between fox and grass
   isFox = !isFox; 
  }
  
  if (isBunny)
  {
    //rando color and value
    setColor(WHITE); 
    setValueSentOnAllFaces(3);
  }
  if (isFox)
  {
    setColor(ORANGE);
    setValueSentOnAllFaces(4);
  }
  
  if (isBunny == false && buttonSingleClicked())
  {
    //single click a green blink to transition
    transition = true;
  }
  if (isFox == false && buttonSingleClicked())
  {
    //single click a green blink to transition
    transition = true;
    
  }
  
  //wipe single clicks
  buttonSingleClicked();
  
  if(transition)
  {
    
   setValueSentOnAllFaces(5);
    
  //change grass to bunny
  FOREACH_FACE(f)
  {
   if (getLastValueReceivedOnFace(f) == 3 && isBunny == false)
    {
     isBunny = true; 
    }
  } 
    
  //change grass to fox
  FOREACH_FACE(f)
  {
   if (getLastValueReceivedOnFace(f) == 4 && isFox == false)
    {
     isFox = true;
    }
  }
    
    //close gate
    transition = false;
  }
  
  //if i was a a white blink and i am receiving a signal to transition, shut off
  FOREACH_FACE(f)
  {
   if (getLastValueReceivedOnFace(f) == 5 && isBunny == true)
    {
     isBunny = false; 
    }
  }
  FOREACH_FACE(f)
  {
   if (getLastValueReceivedOnFace(f) == 5 && isFox == true)
    {
     isFox = false; 
    }
  }

}

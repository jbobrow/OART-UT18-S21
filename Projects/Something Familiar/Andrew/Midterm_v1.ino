//Andrew Tebbe
/*
 *  if (third == 1)
  {
     setColor(WHITE);
  } else if (third == 2)
  {
   setColor(RED);
  } else if (third == 3)
  {
   setColor(ORANGE);
  } else 
  {
   setColor(GREEN);
  }
 */
enum Mode {
  SETUP,  // = 0
  PATTERN,  // = 1
  INPUT,  // =2
  END // =3
};

byte mode;

Color colors[4] = {WHITE, ORANGE, RED, GREEN};
byte myColorIndex =  0;
Color myColor = colors [myColorIndex];
byte first;
byte second;
byte third;
byte fourth;

void setup() {
  mode = SETUP;
  
}

void loop() {

 switch ( mode ) {
 
  case SETUP: //beforre gamme starts
     setupLoop();
     break;
 
  case PATTERN: //initial pattern
    patternLoop();
    break;
   
  case INPUT: //where player guesses the pattern
    inputLoop();
    break;
   
  case END:  //end state
    endLoop();
    break;

  }
}

void setupLoop()
{
  first = random(2);
  bool same = true; 
  while(same) //these whiles make sure that the same color is not picked twice
  {
    second = random(2);
      if(second!=first)
      {
        same = false;
      }
  }
   same = true; 
  while(same)
  {
    third = random(2);
      if(third!=first && third != second) 
      {
        same = false;
      }
  }
   if(buttonDoubleClicked() ) //starts the pattern
       {
        mode = PATTERN;
       fourth = 3; //THIS COLOR WILL ALWAYS BE GREEN
       }
setColor(OFF); //base color
}


void patternLoop()
{ //trying   to  make  the  positions of  ball (white) hole (orange) hazard (red) be random.  
  //want to find a  way to use setColor for individual blinks
  setColorOnFace(colors[first], 0);
  setColorOnFace(colors[second], 1);
  setColorOnFace(colors[third], 2);
  setColorOnFace(colors[fourth], 3);
  setColorOnFace(colors[fourth], 4);
  setColorOnFace(colors[fourth], 5);
  //HOOW TO SET COLOR ON INDIVIDUAL BLINKS? MAKE 1 ONLY RED, ONE BLUE, ETC
}
void inputLoop() //STRAIGHT LINE CODE
{
  if(buttonSingleClicked() )
  {
    setValueSentOnAllFaces(2);
  }
  FOREACH_FACE(f) 
  {
    if (getLastValueReceivedOnFace(f) == 2 ) //everyone checks this
    {
      setColor(RED); 
      setValueSentOnAllFaces(2); //makes it so it goes the entire way
    }
  } 
}
/* //MOVES THE BALL ACROSS THE BLINKS
 *  #define length 2000
Timer test;
 bool check = false; 

void setup() {
    setColor(WHITE); //DO IN STRAIGHT LINE
   
}

void loop() {
  if(buttonSingleClicked() )
  {
   setValueSentOnAllFaces(2);
   test.set(length);
 
  }
  
  FOREACH_FACE(f) 
  {
    if(isValueReceivedOnFaceExpired(f) ) //
    {
        continue; 
      
    }
    if (getLastValueReceivedOnFace(f) == 2 && check == false) //everyone checks this
    {
      test.set(length);
      setColor(RED); 
      setValueSentOnAllFaces(2); //makes it so it goes the entire way
      check = true;
    }
  }
  
   if(test.isExpired() && check == true)
   {
         setValueSentOnAllFaces(1); 
         setColor(WHITE); 
     }
  
}


 */

void endLoop() //ending case to reset the game
{

}

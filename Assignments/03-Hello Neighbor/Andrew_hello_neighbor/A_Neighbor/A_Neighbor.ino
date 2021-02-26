

enum Mode {
  SETUP,  // = 0
  PLAY,  // = 1,  
  END // =2
};

byte mode;
Color faceColors[6] = {RED, BLUE, RED, RED, BLUE, BLUE};
Color colors[2] = {RED, BLUE};
byte faceColorIndices[6] = {0, 1, 0, 0, 1, 1};
Color testColor = colors[0];
byte startColor;

void setup() {
  mode = SETUP;
}

void loop()
{
  switch ( mode )
  {
  case SETUP: //beforre gamme starts
     setupLoop();
     break;
 
  case PLAY: //initial pattern
    playLoop();
    break;
   
    case END:  //end state
    endLoop();
    break;
  }
}

  void setupLoop()
{
    setColor(GREEN);
    startColor = random(5); 
    if(buttonDoubleClicked() ) //starts the game
    {
          setColor(colors[faceColorIndices[startColor]]);//randomly sets a base color onto all the blinks
          mode = PLAY;
    }
}
   
void playLoop()
{
 
  FOREACH_FACE(f) //NOT CONNECTED
  {
   //setValueSentOnFace(f, f);
   //setColor(BLUE);
  }

  FOREACH_FACE(f)
  {
    if (!isValueReceivedOnFaceExpired(f)) // CONNECTED
    {
      byte neighborVal = getLastValueReceivedOnFace(f);
      if(startColor == 0 || startColor == 2 || startColor == 3 )
      {
      setColorOnFace(BLUE, f);
      } else
      {
        setColorOnFace(RED,f);
      }
    }
  }
}

void endLoop()
{
  
}

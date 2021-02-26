//Andrew Tebbe, Puzzle Game, 2/19/2021

enum Mode {
  SETUP,  // = 0
  PATTERN,  // = 1
  INPUT,  // =2
  END // =3
};

byte mode;

Color colors[3] = {ORANGE, GREEN, CYAN};
byte myColorIndex =  0;
Color myColor = colors [myColorIndex];
byte first;
byte second;
byte third;

Timer gameTimer;
Timer time1;
Timer time2;
#define GAME_DURATION 6000
#define time 2000
#define time11 4000

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
        gameTimer.set (GAME_DURATION); //full sequence
        time1.set (time); //first time
        time2.set (time11);  //second time
       }
setColor(BLUE); //base color
}


void patternLoop()
{
  setColor(colors[first]); //first coloor in sequence
  if( time1.isExpired() ) {
    setColor(colors[second]); //2nd color in sequence
  }
  if( time2.isExpired() ) {
    setColor(colors[third]); //3rd color in sequence
  }
  if (gameTimer.isExpired()) {
    mode = INPUT;
  }
}
void inputLoop()
{
setColor(BLUE); //return to base coloor
  if(buttonDoubleClicked() && second == 1) //correct state for 2nd spot
    {
      setColor(GREEN);
      mode = END;
    } 
    if(buttonSingleClicked() && first == 1) //correct for 1st spot
    {
      setColor(GREEN);
      mode = END;
    } 
    if(buttonMultiClicked() && third ==1) //3rd correct spot
    {
      setColor(GREEN);
      mode = END;
    } 
    
}


void endLoop() //ending case to reset the game
{
  bool longPressing = false;
  if (buttonLongPressed())
  {
    longPressing = true;
    mode = SETUP;
  }
}

/*
   if (buttonDoubleClicked() && second == 0 ) //NONE OF THE FAIL CONDITIONS WORK. THEY DO NOT FIRE PROPERLY. 
    {
      setColor(RED);
      mode = END;
    }
      if (buttonDoubleClicked() && second == 2 )
    {
      setColor(RED);
      mode = END;
    }

 */

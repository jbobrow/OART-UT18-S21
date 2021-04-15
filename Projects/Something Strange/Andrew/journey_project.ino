byte faceBri[6];

#define DECREMENT 5
#define FADE_INTERVAL 20

byte index = 0;
byte prev_index = 0;
Timer fadeTimer;

enum Mode {
  START,  // = 0
  GOAL,  // = 1
  INPUT //=2
};
byte mode;


void setup() {
  // put your setup code here, to run once:
  randomize();

     mode = START;
}

void loop() {
  
   switch ( mode ) {
 
  case START: //beforre gamme starts
     startLoop();
     break;
 
  case GOAL: //set the goal target
    goalLoop();
    break;
   
  case INPUT: //where player uses the compasss
    inputLoop();
    break;

  }
}

void startLoop()
{
   if(buttonMultiClicked())
    {
      setColor(RED); //designate the target
      mode = GOAL;
    } 
 
   if(buttonDoubleClicked() )
   {
      mode = INPUT; //starts the game
   }
}

void goalLoop()
{
    if(!isAlone())
    {
      setColor(BLUE); //win anim here
    }
    if(buttonLongPressed()) //RESETS  THE GAME
    {
      setColor(OFF);
      mode = START; 
    }
}

void inputLoop()
{
  if (buttonSingleClicked() || buttonDoubleClicked()) { 
    // choose an index that is different from the previous index
   
    //When clicked, send value to neighbor. After, reset value so can do again.
    
    while (index == prev_index) {
      index = random(5);
    }
    faceBri[index] = 255;
    prev_index = index;
  }

  //send value 

  if (fadeTimer.isExpired()) {

    FOREACH_FACE(f) {
      if (faceBri[f] > DECREMENT) {
        faceBri[f] -= DECREMENT;
      }
    }
    fadeTimer.set(FADE_INTERVAL);
  }


  // display our bri
  FOREACH_FACE(f) {
    setColorOnFace(dim(ORANGE, faceBri[f]), f);
  }
}

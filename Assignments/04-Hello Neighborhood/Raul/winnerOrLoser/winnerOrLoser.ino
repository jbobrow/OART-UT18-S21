
//GAME STUFF
enum gameStates = {SETUP, PLAYING, OUTCOME};
byte stateTracker = SETUP;
bool settingUp = true;

//TEAM STUFF
Color teamColors[4] = {BLUE, ORANGE, GREEN, MAGENTA};
byte color;
byte teamOne;
byte teamTwo;

//TIMERS

Timer timeUp;
#define TIME_UP_LENGTH 2500
Timer playTimer;
#define PLAY_LENGTH 40000



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  switch (gameStates)
  {
    case SETUP:
      setupLoop();
      break;
    case PLAYING:
      playingLoop();
      break;
    case OUTCOME:
      outcomeLoop();
      break;
  }

}


void setupLoop ()
{
  //if the blink is alone allow it to toggle between "teams"

  setColor(WHITE);

  if (buttonSingleClicked() && settingUp == true)
  {
    color = (color + 1) % 4;
    setColor(teamColors[color]);
    setValueSentOnAllFaces(6);
  }


  //switch game modes
  if (buttonDoubleClicked())
  {
    settingUp = false;
    gameStates = PLAYING;
  }

}


//a timer for two minutes is set and blinks can be taken away with the alone blinks attatched to them and there is
//something keeping track what is gone, display red when the time is up and wait
void playingLoop ()
{
  playTimer.set(PLAY_LENGTH);

  //the blinks with color should send their array value

  if (____)
  {
    setValueSentOnAllFaces(color);
  }

  //if a white blink receives input from another one with color, change it to that color
  FOREACH_FACE(f)
  {
    if (getLastValueReceivedOnFace(f) == color)
    {
      setColor(color);
    }
  }



  //when the play timer expires, switch gamestates and display red for a sec

  if (playTimer.isExpired())
  {
    timeUp.set(TIME_UP_LENGTH);
    if (!timeUp.isExpired)
    {
      // display red on all the faces that have not been taken aka if the face is white
      //11 is a rando value
      setValueSentOnAllFaces(11);
      if (getLastValueReceivedOnFace(f) == 11)
      {
        setColor(RED);
        setValueSentOnAllFaces(11);
      }
    }
    gameStates = OUTCOME;
  }



}

void outcomeLoop ()
{

  //display the winning color aka the one that had the most taken away





  //display a tie

  if (teamOne == teamTwo)
  {
    setColor(OFF)
    setColorOnFace(YELLOW,((millis() / 190) % 6)) 
  } 



  //reset Game

  if (buttonLongPressed())
  {
    gameStates = SETUP;
  }


}

int rand;
Timer timerGame;
bool success;
int round;
Timer zzTimer;
bool gameEnd;


void setup() 
{
  randomize();
  setColor(OFF);
}

void loop() 
{
  // Start Game
  if (buttonDoubleClicked()) 
  { 
    setupGame();
  }

  // Continue to next Stage if half a second delay is over
  if (zzTimer.isExpired() && !timerGame.isExpired())
  {
      setupGame();
  }

  // Game is still Running
  if (timerGame.isExpired() != true) 
  { 
    // Clicked Blink
    if (buttonSingleClicked()) 
    {
      // Clicked Green
      if (success == true) 
      {
        setColor(MAGENTA);
        // Wait half a Second
        zzTimer.set(500);
      }

      // Clicked Red
      if (success == false) 
      {
        setColor(RED);
        // Wait half a second - end game
        gameEnd = true;
      }
    }
  }

  // If timer has expired
  if (timerGame.isExpired())
  {
    // If button was red, and not clicked
    if (success == false && !gameEnd)
    {
      // Wait half a second then continue to next stage
      zzTimer.set(500);
      setupGame();
    }
    // Button was red and clicked or green and not clicked
    else
    {
      setColor(OFF);
    }
  }
}

// Initial Setup
void setupGame()
{ 
    //startgame
    timerGame.set(2000);
    rand = random(1);
      zzTimer.set(20000000);
      gameEnd = false;

    if (rand == 0) 
    {
      setColor(GREEN);
      success = true;
    }
    else 
    {
      setColor(BLUE);
      success = false;
    }
}

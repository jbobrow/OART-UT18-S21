enum States {
  ALIVE,
  DEAD
};

int state = ALIVE;
Timer delayTimer;

void setup() {
  // put your setup code here, to run once:
  state = random(1);
}

void loop() {
  // put your main code here, to run repeatedly:

  //if I single click, cell becomes alive
  if (buttonSingleClicked())
  {
    if(state == ALIVE) {
      state = DEAD;
    }
    else {
      state = ALIVE;
    }
  }


  if (delayTimer.isExpired())
  {
    delayTimer.set(300);

    // check my suroundings
    switch (state) {
      case ALIVE: aliveCheck(); break;
      case DEAD: deadCheck(); break;
    }
  }
  
      // display my state
    switch (state) {
      case ALIVE: setColor(MAGENTA); break;
      case DEAD: setColor(WHITE); break;
    }

    // share my state
    setValueSentOnAllFaces(state);

}

byte numNeighborsAlive() {
  byte count = 0;

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte neighborVal = getLastValueReceivedOnFace(f);
      if (neighborVal == ALIVE) {
        count++;
      }
    }
  }

  return count;
}

void aliveCheck()
{

  //count alive neighbors
  byte aliveCount  = numNeighborsAlive();

  if (aliveCount  < 2) {
    state = DEAD;
  }

  // any live cell with 2 or 3 neighbors lives on to the next generation
  if (aliveCount == 2 || aliveCount == 3)
  {
    state = ALIVE;
  }

  // any live cell with more than 2 or 3 neighbors dies
  if (aliveCount > 3)
  {
    state = DEAD;
  }
}

void deadCheck ()
{
  //count alive neighbors
  byte aliveCount  = numNeighborsAlive();

  //any dead cell with exactly 3 alive neighbors becomes alive
  if (aliveCount == 2)
  {
    state = ALIVE;
  }
}
